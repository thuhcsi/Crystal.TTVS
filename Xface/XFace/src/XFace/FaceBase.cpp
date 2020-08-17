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
 * The Original Code is Xface Core Library; FaceBase imp.
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


#include <XFace/FaceBase.h>

#include <iostream>
#include <XEngine/RendererGL.h>
#include <XFace/RaisedCosInfluence.h>
#include <XEngine/TextureManager.h>

namespace XFace{

FaceBase::FaceBase(void) : m_pRenderer(0), m_pFDP(0), m_rotateX(0), m_rotateY(0), m_rotateZ(0)
{
}

FaceBase::~FaceBase(void)
{
	destroy();
}

void FaceBase::destroy()
{
	// clear the drawables
	std::for_each(m_Drawables.begin(), m_Drawables.end(), deletePointer_functor<Drawable>());
	m_Drawables.clear();
	m_RenderList.clearList();

	delete m_pRenderer;
	m_pRenderer = 0;
	
	delete m_pFDP;
	m_pFDP = 0;
}

bool FaceBase::initMeshes(const std::list<MeshInfo>& filenames, const std::string& path)
{
	// load new ones
	std::list<MeshInfo>::const_iterator it = filenames.begin();
	while (it != filenames.end())
	{
		std::string total_path = path + it->path;
		// we do not use the format, we resolve it inside the loader
		std::list<Drawable*> drawables = ModelFileFactory::loadModelFile(it->file, total_path);
		if(drawables.empty())
		{
			std::cerr << "Error loading mesh file " << it->file << std::endl;
			return false;
		}
		else
		{
			m_Drawables.insert(m_Drawables.end(), drawables.begin(), drawables.end());
			std::for_each(drawables.begin(), drawables.end(), renderListFiller_functor(m_RenderList));
		}

		++it;
	}
	
	return true;
}

bool FaceBase::initAux()
{
	// create the renderer (GL or DX choice here)
	m_pRenderer = new RendererGL;

	// initializes influence calcs
	initInfluenceCalculators();

	return true;
}

bool FaceBase::init(std::istream& input)
{
	FDPFile fdp_file;
	delete m_pFDP;
	m_pFDP = new FDP;
	fdp_file.load(input, m_pFDP);

	// initialize the textures and meshes (asuming path is correct as it is)
	if( !initMeshes(fdp_file.getMeshList(), "./") )
		return false;

	return initAux();
}

bool FaceBase::init(const std::string& filename, const std::string& path)
{
	// load configuration and fdp
	// FDP items load
	FDPFile fdp_file;
	delete m_pFDP;
	m_pFDP = new FDP;
	fdp_file.load(path + filename, m_pFDP);

	// initialize the textures and meshes
	if( !initMeshes(fdp_file.getMeshList(), path) )
		return false;

	return initAux();
}

void FaceBase::initInfluenceCalculators()
{
	// we have to make this function call in order to make static variables initialized in influence site
	RaisedCosInfluenceSph def(0, 5);

	// clean the old (if any) drawable contents (bones)
	std::for_each(m_Drawables.begin(), m_Drawables.end(), skinResetter_functor(MeshManager::getInstance()));

	const FDP::FDPITEMS items = m_pFDP->getItems();
	// calls another for_each inside for initializing weights in influence calc. and transferring them to skinned meshes
	std::for_each(items.begin(), items.end(), deformationInitializer_functor(MeshManager::getInstance()));
}

void FaceBase::render()
{
	m_pRenderer->setRotation(m_rotateX, m_rotateY, m_rotateZ);
	m_RenderList.renderList(m_pRenderer);
}

void FaceBase::update(const std::vector<float>& FAPs)
{
	std::for_each(m_Drawables.begin(), m_Drawables.end(), skinDeformer_functor(MeshManager::getInstance(), FAPs));

	// apply global rotation
	m_rotateX = FAPs[47]*100.001f;
	m_rotateY = FAPs[48]*100.001f;
	m_rotateZ = FAPs[49]*100.001f;
}

void FaceBase::rebindTextures()
{
    TextureManager::getInstance()->rebindTextures();
}

} // namespace XFace
