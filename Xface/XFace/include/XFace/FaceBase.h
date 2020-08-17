/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Xface Core Library.
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.itc.it) Trento / ITALY.
 * For info, contact: xface-info@itc.it or http://xface.itc.it
 * Portions created by the Initial Developer are Copyright (C) 2004
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@yahoo.com)
 * ***** END LICENSE BLOCK ***** */

/*! 
	\file	FaceBase.h
	\brief	Main face object.
*/

#ifndef FACEBASE_H_
#define FACEBASE_H_

#include <string>
#include <map>
#include <vector>

#include <XEngine/RenderList.h>
#include <XEngine/Drawable.h>
#include <XEngine/IRenderer.h>
#include <XFace/IInfluenceCalculator.h>
#include <XFace/FDP.h>
#include <XFace/FDPFile.h>
#include <XEngine/MeshManager.h>

using XMath::Vector3;
using namespace XEngine;

namespace XFace{

/*!
	\brief Main XFace class.
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   June 2003

	Main entry point for XFace library related operations. Only exposing this class
	to users should be sufficient. Pass the FDP configuration file for initialization, 
	then call update and render when needed. 
	\code
	    FaceBase myFace;
	    myFace.init(FDPFilename, FDPPath); // load an FDP file
	    
	    // in a loop of all FAP data
	    myFace.update(FAPData);
	    myFace.render();
	    
	    // when done, it's good practice to cleanup, especially if you plan on loading
	    // a new FDP file over the same FaceBase instance.
	    myFace.destroy();    
	\endcode
	
	\note Alternatively, you can use XFaceApp module and use XML tasks as messages 
	(see related documentation) and forget about this class and all other XFace classes.
*/
class FaceBase
{
	/*!
		for each Drawable*, inserts them to RenderList
	*/
	struct renderListFiller_functor
	{
	private:
		RenderList& m_rl;
	public:
		renderListFiller_functor(RenderList& rl): m_rl(rl){}
		void operator() (Drawable* dr) const{
			m_rl.addDrawable(dr);
		}

	};

	/*!
	*/
	struct skinDeformer_functor
	{
	private:
		const MeshManager* const m_pMM;
		const std::vector<float>& m_FAPs;
	public:
		skinDeformer_functor(const MeshManager* const pMM, const std::vector<float>& FAPs) : m_pMM(pMM), m_FAPs(FAPs) {}
		void operator()(const Drawable* const dr) const{
			DeformableGeometry* pMesh = m_pMM->getMesh(dr->getMeshName());
			pMesh->updateAnimationParams(m_FAPs);
		}
	};
	
	/*!
		Cleans the old (if any) drawable contents (bones)
	*/
	struct skinResetter_functor
	{
	private:
		const MeshManager* const m_pMM;
	public:
		skinResetter_functor(const MeshManager* const pMM) : m_pMM(pMM) {}
		void operator()(const Drawable* const dr) const{
			DeformableGeometry* pMesh = m_pMM->getMesh(dr->getMeshName());
			pMesh->clearInfluences();
		}
	};
	
	/*!
		for each FDPItem, retrieves the member IInfluenceCalculator*'s, and executes deformerInitializer_functor on them.
		Then, after accumulating the weights, passes the bone info to DeformableGeometry objects in m_Drawables.
	*/
	struct deformationInitializer_functor
	{
	private:
		/*!
			for each std::pair<IInfluenceCalculator* const, std::string> (actually IInfluenceCalculator*), 
			calculates the weights in IInfluenceCalculator instances. Then transfers these weights to SkinnedMesh obj.
		*/
		struct influenceToSkin_functor
		{
		private:
			DeformableGeometry* const m_pMesh;
			const FDPItem* const m_pFdp;
		public:
			influenceToSkin_functor(DeformableGeometry* const pMesh, const FDPItem* const pFdp) : m_pMesh(pMesh), m_pFdp(pFdp) {}
			void operator() (const std::pair<IInfluenceCalculator* const, std::string>& inf) const{
				inf.first->init(m_pFdp);
				m_pMesh->addInfluence(m_pFdp->getAOI(), inf.first->getWeights(), inf.first->getFapID());
			}
		};
	
		MeshManager* m_pMM;
	public:
		deformationInitializer_functor(MeshManager* pMM) : m_pMM(pMM){}
		void operator()(const FDPItem* fdp) const{
			FDPItem::INFLUENCECALC_MAP infCalcs =  fdp->getInfluenceCalculators();
			DeformableGeometry* pMesh = m_pMM->getMesh(fdp->getAffects());
			std::for_each(infCalcs.begin(), infCalcs.end(), influenceToSkin_functor(pMesh, fdp));
		}
	};

	/*!
		template fonctor to call delete on collection that stores pointers to objects.
	*/
	template <class T>
	struct deletePointer_functor
	{
		void operator()(T* p) const {delete p;}
	};
	
	float m_rotateX, m_rotateY, m_rotateZ;
	bool initAux();
protected:
	FDP* m_pFDP;
	RenderList m_RenderList;
	std::list<Drawable*> m_Drawables;
	IRenderer* m_pRenderer;

public:
	const FAPU& getFAPU() const {return m_pFDP->getFAPU();};
	FDP* getFDP() const {return m_pFDP;}
	const std::list<Drawable*>& getDrawables() const {return m_Drawables;}
	void render();
	void destroy();
	virtual void initInfluenceCalculators();
	bool initMeshes(const std::list<MeshInfo>& filenames, const std::string& path);
	virtual bool init(std::istream& input);
	virtual bool init(const std::string& filename, const std::string& path = "./");
	void update(const std::vector<float>& FAPs);
	FaceBase(void);	
	virtual ~FaceBase(void);
    void rebindTextures();
};
}
#endif // FACEBASE_H_
