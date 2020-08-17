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
 * The Original Code is XFace::XEngine
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.itc.it) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@yahoo.com)
 * ***** END LICENSE BLOCK ***** */
/*! 
	\file DeformableGeometry.h
	\brief Deformation enabled geometry class.
*/

#pragma once
#include "IndexedFaceSet.h"
#include <cassert>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

namespace XEngine{

#define MAXWEIGHT 8

/*!
	\brief Deformation enabled geometry class.
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   Sept 2004
			
	Deformation is done on this Geometry derived class.
*/
class DeformableGeometry :
	public IndexedFaceSet
{
	struct DeformationWeights
	{
		DeformationWeights() {memset(&w[0], 0, MAXWEIGHT*sizeof(float));}
		float operator[](size_t i) const {assert(i < MAXWEIGHT); return w[i];}
		float& operator[](size_t i) {assert(i < MAXWEIGHT); return w[i];}
	private:
		float w[MAXWEIGHT];
	};

	struct DeformationIndices
	{
		DeformationIndices() {memset(&id[0], 0, MAXWEIGHT*sizeof(unsigned short));}
		unsigned short operator[](size_t i) const {assert(i < MAXWEIGHT); return id[i];}
		unsigned short& operator[](size_t i) {assert(i < MAXWEIGHT); return id[i];}
	private:
		unsigned short id[MAXWEIGHT];
	};

	// these should be static
	std::vector<Vector3> m_FAPDirection;
	void prepareFAPLookup();
	bool m_bNeedUpdate;
protected:
	std::vector<DeformationWeights> m_weights;
	std::vector<DeformationIndices> m_weightIndices;
	std::vector<Vector3> m_def_vertices;
	std::vector<float> m_animationParams;
public:
	const Vector3* const getDeformedVertices() const {return &m_def_vertices[0];}
	void setVertices(Vector3* pVert, unsigned int size);
	void setVertices(std::vector<Vector3> &vertices);
	void addInfluence(const std::set<unsigned short>& aoi, const std::vector<float>& w, unsigned short fap);
	void clearInfluences();
	void updateAnimationParams(const std::vector<float>& aps) {m_animationParams = aps; m_bNeedUpdate = true;}
	bool needUpdate() const {return m_bNeedUpdate;}
	void update();
	DeformableGeometry(const std::string& name);
	~DeformableGeometry(void);
};

} // namespace XEngine

/*!
	\page DeformationPage Skinning Adventures / Architecture Review

	\section DevDiary Developer Diary
	\date Wednesday, September 15, 2004

	I finally finished working on skinning. First of all, I didn't really implement the conventional vertex 
	skinning, because I realized that I don't have to!

	I was planning to use ARB_vertex_blend OpenGL extension and implement a mid-level path along with hw shader 
	and sw deformer. Unfortunately, I realized that, this path requires some change in the mesh data structure. 
	I needed to have submeshes connected to all deformers, something that I don't want to implement really. 
	Actually, my current structure is much better than that already, and also both hw and sw paths do not need 
	that kind of structuring. Alternatively, you can use ARB_matrix_palette extension along with vertex_blend to 
	overcome this, but it is not supported by any of the card vendors anymore! People say, this is because 
	skinning by extensions is replaced by hw shader versions. So, as a result, I also decided to forget about 
	this path totally.

	As soon as I gave up that path, I realized that I do NOT need to change my architecture that much, I do NOT 
	need to implement skinning in the way it is explained. I don't need to store transformation matrices and bone 
	structures for every deformation point as I planned. Here is my architecture.

	XFace::FaceBase class controls the whole system as before. We have a new class for geometry to handle the 
	deformations, XEngine::DeformableGeometry. Each XEngine::Drawable object now has a XEngine::DeformableGeometry 
	bound to it. As before, XEngine::Drawable's represent the building blocks of the head, i.e. the face, hair, 
	tongue, etc.. FaceBase, has a list of XEngine::Drawable objects, during the initialization phase, the meshes 
	are loaded from disk into XEngine::DeformableGeometry objects and attached to XEngine::Drawable objects. 
	In reality, only the names of XEngine::DeformableGeometry objects are stored in XEngine::Drawable, and they 
	are retrieved through XEngine::MeshManager singleton object. During the initialization phase, also FDP file 
	is loaded and XFace::FDPItems are filled. XFace::FDPItem objects represent MPEG-4 feature points defined by 
	the standard, and store the characteristics of the feature point. Namely, we store, for each feature point;

    * the mesh it is affecting,
    * control point that represents the feature point,
    * the region (vertices) influenced by the control point,
    * a set of influence calculators

	Influence calculators store weights for each vertex in the zone of influence, where weight represents how 
	the coresponding vertex will be affected by the deformation of that feature point. Interface for the influence 
	calculators are defined by XFace::IInfluenceCalculators class. Currently, we have a set of raised cosine based 
	influence calculator concrete classes. XfaceEd automatically detects influence calculators, thanks to pluggable 
	factory pattern implemented inside. See XFace::InfluenceCalculatorMaker for more information on that. 
	Each XFace::IInfluenceCalculator derived object also stores a coefficient to control the deformation limits, 
	and the FAP id that the weights are used for (or in other words, the FAP that the influence calculator 
	responds to).

	So during the initialization, when the FDP file is loaded, first static meshes (OBJ/VRML/X3D or any other 
	supported format) are loaded to XEngine::DeformableGeometry instances, and feature point info are loaded to 
	XFace::FDPItems, and XFace::IInfluenceCalculators are created and the weights for the zone of influence are
	calculated. Then those weights are routed to XEngine::DeformableGeometry objects.

	XEngine::DeformableGeometry objects store two sets of vertices, one for the still (static, no emotion/movement)
	and one for the deformed. During runtime, when playback of FAPs starts, for each new frame, 68 FAP values are
	sent to XEngine::DeformableGeometry::updateAnimationParams method, but the deformation is delayed until the
	rendering. During the rendering process, if a geometry needs an update, its update method is called for the 
	moment, though I plan to move this piece of code out of XEngine::DeformableGeometry. This lazy evaluation 
	(in the rendering pipeline) enables us to call a shader instead of updating on software. Though I haven't 
	implemented the shader pipeline, all the structuring is done.
*/
