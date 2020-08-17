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
 * The Original Code is Xface Core Library; Influences.
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

#include <XFace/RaisedCosInfluence.h>
#include <XFace/FDPItem.h>

#include <cassert>
#include <cmath>

#include <XEngine/MeshManager.h>

namespace XFace{

	const RaisedCosInfluenceSphMaker RaisedCosInfluenceSphMaker::m_regRaisedCosInfluenceSph;
	const RaisedCosInfluenceWaveXMaker RaisedCosInfluenceWaveXMaker::m_regRaisedCosInfluenceWaveX;
	const RaisedCosInfluenceWaveYMaker RaisedCosInfluenceWaveYMaker::m_regRaisedCosInfluenceWaveY;
	const RaisedCosInfluenceWaveZMaker RaisedCosInfluenceWaveZMaker::m_regRaisedCosInfluenceWaveZ;
	

	void RaisedCosInfluence::init(const FDPItem* const pFDP)
	{
		calculateDistances(pFDP);
	}
	
	float RaisedCosInfluence::calculateDistances(const FDPItem* const pFDP)
	{
		float maxDist = 0;
		m_weights.clear();
		
		const Vector3* pV = XEngine::MeshManager::getInstance()->getMesh(pFDP->getAffects())->getVertices();
		assert(pV && "RaisedCosInfluenceSph::calculateDistances() could not find any vertices");

		Vector3 pivot = pV[pFDP->getIndex()];
		FDPItem::AOI_STORE indices = pFDP->getAOI();
		FDPItem::AOI_STORE::const_iterator ind_it = indices.begin();
		// for each vertex in area of interest, calculates the distance to the pivot (control) point
		// also calculates the farthest point
		for(; ind_it != indices.end(); ++ind_it)
		{
			float val = calculateDistance(pivot, pV[*ind_it]);
			m_weights.push_back(val);
			if(val > maxDist)
				maxDist = val;
		}

		// calculate directly the raised cosine, not the distance only (nice optimization)
		std::for_each(m_weights.begin(), m_weights.end(), weight_functor(maxDist, m_coef));
		
		return maxDist;
	}

	float RaisedCosInfluenceSph::calculateDistance(const Vector3& p1, const Vector3& p2)
	{
		return (float)sqrt((p1.x - p2.x)*(p1.x - p2.x) 
					+ (p1.y - p2.y)*(p1.y - p2.y)
					+ (p1.z - p2.z)*(p1.z - p2.z) );
	}

	
	float RaisedCosInfluenceWaveX::calculateDistance(const Vector3& p1, const Vector3& p2)
	{
		return (float)fabs(p1.x - p2.x); 
	}

	float RaisedCosInfluenceWaveY::calculateDistance(const Vector3& p1, const Vector3& p2)
	{
		return (float)fabs(p1.y - p2.y); 
	}

	float RaisedCosInfluenceWaveZ::calculateDistance(const Vector3& p1, const Vector3& p2)
	{
		return (float)fabs(p1.z - p2.z); 
	}
} // namespace XFace