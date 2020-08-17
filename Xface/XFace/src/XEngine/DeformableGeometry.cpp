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

#include <XEngine/DeformableGeometry.h>
#include <cassert>
#include <iterator>

namespace XEngine{

DeformableGeometry::DeformableGeometry(const std::string& name) : IndexedFaceSet(name), m_bNeedUpdate(false)
{
	prepareFAPLookup();
}

DeformableGeometry::~DeformableGeometry(void)
{
}

void DeformableGeometry::setVertices(Vector3* pVert, unsigned int size)
{
	// let base class has to do whatever it has to do
	Geometry::setVertices(pVert, size);
	m_def_vertices = m_vertices;

	clearInfluences();
}

void DeformableGeometry::setVertices(std::vector<Vector3> &vertices)
{
	// let base class has to do whatever it has to do
	Geometry::setVertices(vertices);
	m_def_vertices = vertices;

	clearInfluences();
}

void DeformableGeometry::clearInfluences()
{
	m_weights.clear();
	m_weightIndices.clear();
	DeformationWeights val;
	std::fill_n(std::back_inserter(m_weights), m_vertices.size(), val);
	DeformationIndices ind;
	std::fill_n(std::back_inserter(m_weightIndices), m_vertices.size(), ind);
}

void DeformableGeometry::addInfluence(const std::set<unsigned short>& aoi, 
									const std::vector<float>& w, 
									unsigned short fap)
{
	// set the weight and bone id from the data
	std::set<unsigned short>::const_iterator index_it = aoi.begin();
	for (size_t i = 0; i < w.size(); ++i, ++index_it)
	{
		// get an empty slot in weight (limit is MAXWEIGHT weights per vertex)
		size_t slot = 0;
		while(slot< MAXWEIGHT && m_weights[*index_it][slot])
			slot++;
		if(slot == MAXWEIGHT) // this vertex is full of weights, ignore the rest..
			continue;

		// assign finally!
		m_weights[*index_it][slot] = w[i];
		m_weightIndices[*index_it][slot] = fap;
	}
}

/*!
	\todo computeVertexNormals() ??
*/
void DeformableGeometry::update()
{
	for(size_t i = 0; i < m_def_vertices.size(); ++i)
	{
		m_def_vertices[i] = m_vertices[i];
		for(size_t j = 0; j < MAXWEIGHT; ++j)
		{
			if(m_weightIndices[i][j] == 0)
				continue;
			float fap = m_animationParams[ m_weightIndices[i][j] ];
			if(fap)
				m_def_vertices[i] += m_FAPDirection[ m_weightIndices[i][j] ] * (fap * m_weights[i][j]);
		}
	}

//	computeVertexNormals();
}

void DeformableGeometry::prepareFAPLookup()
{
	// first two faps (high level)
	m_FAPDirection.push_back(Vector3(0, 0, 0));		// 1. viseme
	m_FAPDirection.push_back(Vector3(0, 0, 0));		// 2. emotion
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 3. Open_jaw
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 4. lower_t_midlip
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 5. Raise_b_midlip
	m_FAPDirection.push_back(Vector3(1, 0, 0));		// 6. Stretch_l_cornerlip
	m_FAPDirection.push_back(Vector3(-1, 0, 0));	// 7. Stretch_r_cornerlip
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 8. Lower_t_lip_lm
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 9. Lower_t_lip_rm
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 10. Raise_b_lip_lm
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 11. Raise_b_lip_rm
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 12. Raise_l_cornerlip
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 13. Raise_r_cornerlip
	m_FAPDirection.push_back(Vector3(0, 0, 1));		// 14. Thrust_jaw
	m_FAPDirection.push_back(Vector3(-1, 0, 0));	// 15. Shift_jaw
	m_FAPDirection.push_back(Vector3(0, 0, 1));		// 16. Push_b_lip
	m_FAPDirection.push_back(Vector3(0, 0, 1));		// 17. Push_t_lip
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 18. Depress_chin
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 19. Close_t_l_eyelid
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 20. Close_t_r_eyelid
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 21. Close_b_l_eyelid
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 22. Close_b_r_eyelid
	m_FAPDirection.push_back(Vector3(1, 0, 0));		// 23. Yaw_l_eyeball
	m_FAPDirection.push_back(Vector3(1, 0, 0));		// 24. Yaw_r_eyeball
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 25. Pitch_l_eyeball
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 26. Pitch_r_eyeball
	m_FAPDirection.push_back(Vector3(0, 0, 1));		// 27. Thrust_l_eyeball
	m_FAPDirection.push_back(Vector3(0, 0, 1));		// 28. Thrust_r_eyeball
	m_FAPDirection.push_back(Vector3(1, 1, 1));		// 29. Dilate_l_pupil (grow)
	m_FAPDirection.push_back(Vector3(1, 1, 1));		// 30. Dilate_r_pupil (grow)

	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 31. Raise_l_i_eyebrow
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 32. Raise_r_i_eyebrow
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 33. Raise_l_m_eyebrow
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 34. Raise_r_m_eyebrow
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 35. Raise_l_o_eyebrow
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 36. Raise_r_o_eyebrow
	m_FAPDirection.push_back(Vector3(-1, 0, 0));	// 37. Squeeze_l_eyebrow
	m_FAPDirection.push_back(Vector3(1, 0, 0));		// 38. Squeeze_r_eyebrow
	m_FAPDirection.push_back(Vector3(1, 0, 0));		// 39. Puff_l_cheek
	m_FAPDirection.push_back(Vector3(-1, 0, 0));	// 40. Puff_r_cheek
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 41. Lift_l_cheek
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 42. Lift_r_cheek
	m_FAPDirection.push_back(Vector3(-1, 0, 0));	// 43. Shift_tongue_tip
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 44. Raise_tongue_tip
	m_FAPDirection.push_back(Vector3(0, 0, 1));		// 45. Thrust_tongue_tip
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 46. Raise_tongue
	m_FAPDirection.push_back(Vector3(0, 1, 1));		// 47. Tongue_roll
	
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 48. Head_pitch
	m_FAPDirection.push_back(Vector3(1, 0, 0));		// 49. Head_yaw
	m_FAPDirection.push_back(Vector3(-1, 0, 0));	// 50. Head_roll
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 51. Lower_t_midlip_o
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 52. Raise_b_midlip_o
	m_FAPDirection.push_back(Vector3(1, 0, 0));		// 53. Stretch_l_cornerlip_o
	m_FAPDirection.push_back(Vector3(-1, 0, 0));	// 54. Stretch_r_cornerlip_o
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 55. Lower_t_lip_lm_o
	m_FAPDirection.push_back(Vector3(0, -1, 0));	// 56. Lower_t_lip_rm_o
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 57. Raise_b_lip_lm_o
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 58. Raise_b_lip_rm_o
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 59. Raise_l_cornerlip_o
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 60. Raise_r_cornerlip_o

	m_FAPDirection.push_back(Vector3(1, 0, 0));		// 61. Stretch_l_nose
	m_FAPDirection.push_back(Vector3(-1, 0, 0));	// 62. Stretch_r_nose
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 63. Raise_nose
	m_FAPDirection.push_back(Vector3(-1, 0, 0));	// 64. Bend_nose
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 65. Raise_l_ear
	m_FAPDirection.push_back(Vector3(0, 1, 0));		// 66. Raise_r_ear
	m_FAPDirection.push_back(Vector3(1, 0, 0));		// 67. Pull_l_ear
	m_FAPDirection.push_back(Vector3(-1, 0, 0));	// 68. Pull_r_ear
}


} // namespace XEngine


