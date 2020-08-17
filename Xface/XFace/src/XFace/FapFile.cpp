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
 * The Original Code is Xface Core Library; FAP file loader.
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

#include <XFace/FAPFile.h>

#include <fstream>
#include <iostream>
#include <strstream>
#include <string>
#include <iterator>

#include <XFace/FDP.h> // for FAPU

namespace XFace{

FAPFile::FAPFile(void) : m_bLoaded(false), m_currentFAPID(0)
{
}

FAPFile::~FAPFile(void)
{
}

bool FAPFile::open(std::istream& input, const FAPU& fapu)
{
	m_FAPs.clear();
	m_bLoaded = false;

	std::string comment;
	bool init = true;
	bool isMask = true;
	char stupidname[255];
	int mask[68];
	int row = 0;
	while ( !input.fail() )
	{
		if(input.peek() == '#') // skip comment
		{
			std::getline(input, comment);
		}
		else if(init) // header info
		{
			size_t nFAPs;
			input >> m_version;
			input >> stupidname;
			input >> m_FPS;
			input >> nFAPs;

			for (size_t i = 0; i < nFAPs; ++i)
			{
				std::vector<float> fap_set;
				fap_set.reserve(68);
				std::fill_n(std::back_inserter(fap_set), 68, 0.0f);
				m_FAPs.push_back(fap_set);
			}
				
			init = false;
		}
		else if (isMask) // mask
		{
			for (int i = 0; i < 68; ++i)
				input >> mask[i];
			isMask = false;	
		}
		else // fap
		{
			input >> row; // first item is the index of the current FAP (ensures synchronization)
			std::vector<float>& fap_set = m_FAPs[row];
			// then the values
			for (int i = 0; i < 68; ++i)
			{
				if(mask[i] != 0)
					input >> fap_set[i];
			}
			isMask = true;
		}
	}

	scaleFAPs(fapu);
	adjustFAPs();
	
	rewind();
	m_bLoaded = true;
	
	return true;
}

/*!
	This method adjusts the FAP values coming from xml2fap tool. In my opinion (Koray) FAPs 
	has to come to this point already adjusted. But adding this method for a temporary time being.
*/
void FAPFile::adjustFAPs()
{
	for(unsigned long i = 0; i < m_FAPs.size(); ++i)
	{
		if ( m_FAPs[i][51]!=0 && m_FAPs[i][4]==0)		//	8.2 vert
			m_FAPs[i][4]=m_FAPs[i][51];
		if ( m_FAPs[i][4]!=0 && m_FAPs[i][51]==0 )	//	2.3 vert
			m_FAPs[i][51]=m_FAPs[i][4];
		if ( m_FAPs[i][57]!=0 && m_FAPs[i][10]==0)	//	8.8 vert
			m_FAPs[i][10]=m_FAPs[i][57];
		if ( m_FAPs[i][10]!=0 && m_FAPs[i][57]==0)	//	2.9 vert
			m_FAPs[i][57]=m_FAPs[i][10];
		if ( m_FAPs[i][56]!=0 && m_FAPs[i][9]==0)	//	8.7 vert
			m_FAPs[i][9]=m_FAPs[i][56];
		if ( m_FAPs[i][9]!=0 && m_FAPs[i][56]==0)	//	2.8 vert
			m_FAPs[i][56]=m_FAPs[i][9];
	
		if ( m_FAPs[i][50]!=0 && m_FAPs[i][3]==0)	//	8.1 vert
			m_FAPs[i][3]=m_FAPs[i][50];
		if ( m_FAPs[i][3]!=0 && m_FAPs[i][50]==0 )	//	2.2 vert
			m_FAPs[i][50]=m_FAPs[i][3];
		if ( m_FAPs[i][55]!=0 && m_FAPs[i][8]==0)	//	8.6 vert
			m_FAPs[i][8]=m_FAPs[i][55];
		if ( m_FAPs[i][8]!=0 && m_FAPs[i][55]==0)	//	2.7 vert
			m_FAPs[i][55]=m_FAPs[i][8];
		if ( m_FAPs[i][54]!=0 && m_FAPs[i][7]==0)	//	8.5 vert
			m_FAPs[i][7]=m_FAPs[i][54];
		if ( m_FAPs[i][7]!=0 && m_FAPs[i][54]==0)	//	2.6 vert
			m_FAPs[i][54]=m_FAPs[i][7];
			
		if ( m_FAPs[i][5]!=0 && m_FAPs[i][52]==0)			//	2.4 ORIZ. 8.3 ORIZ. =>angoli della bocca
			m_FAPs[i][52]=m_FAPs[i][5];
		if ( m_FAPs[i][52]!=0 && m_FAPs[i][5]==0)
			m_FAPs[i][5]=m_FAPs[i][52];

		if ( m_FAPs[i][6]!=0 && m_FAPs[i][53]==0)			//	2.5 ORIZ. 8.4 ORIZ. =>angoli della bocca
			m_FAPs[i][53]=m_FAPs[i][6];
		if ( m_FAPs[i][53]!=0 && m_FAPs[i][6]==0)
			m_FAPs[i][6]=m_FAPs[i][53];

		if ( m_FAPs[i][11]!=0 && m_FAPs[i][58]==0)			//	2.4 VERT. 8.3 VERT. =>angoli della bocca
			m_FAPs[i][58]=m_FAPs[i][11];
		//if ( m_FAPs[i][58]!=0 && m_FAPs[i][11]==0)
			//{ m_FAPs[i][11]=m_FAPs[i][58]; }							

		if ( m_FAPs[i][12]!=0 && m_FAPs[i][59]==0)			//	2.5 VERT. 8.4 VERT. =>angoli della bocca
			m_FAPs[i][59]=m_FAPs[i][12];
		//if ( m_FAPs[i][59]!=0 && m_FAPs[i][12]==0)
		//	{ m_FAPs[i][12]=m_FAPs[i][59]; }							
		
		//Le seguenti linee permettono di controllare che l'intrusione delle labbra non sia eccessiva
		//arrivando a far vedere i denti.Da notare che come riferimento utilizzo la coordinata z 
		//degli fdp 2.2 e 2.3 parametrizzate rispetto all'unit?MNS.Il fattore 1/4 di cui viene scalato 
		//il valore originario ?empirico.
		//NB: nessun limite per ora (6/6) sulla protusione.
	//	if ( (m_FAPs[i][16]*MNS) < (fdp[2][2][2]*MNS) ) { m_FAPs[i][16]=m_FAPs[i][16]/4; }
	//	if ( (m_FAPs[i][15]*MNS) < (fdp[2][3][2]*MNS) ) { m_FAPs[i][15]=m_FAPs[i][15]/4; }

	}
}


void FAPFile::scaleFAPs(const FAPU& fapu)
{
	for(unsigned long j = 0; j < m_FAPs.size(); ++j)
		scaleFAPVect(m_FAPs[j], fapu);
}

void FAPFile::scaleFAPVect(std::vector<float>& FAPs, const FAPU& fapu)
{
	for(size_t i = 0; i < 68; ++i)
	{
		if( ( (i > 1) && (i < 5) ) || 
			( (i > 6) && (i < 14)) ||
			( (i > 14)&& (i < 18)) ||
			(i == 43) || (i == 45) || (i == 50) || (i == 51) ||
			( (i > 53) && (i < 60)) )
		{
			FAPs[i] *= fapu.MNS;
		}
		else if ( (i == 5) || (i == 6) || (i == 14) || (i == 42) || 
			(i == 44) || (i == 52) || (i == 53) )
			FAPs[i] *= fapu.MW;
		else if ( ( (i > 17) && (i < 22) ) || (i == 28) || (i == 29))
			FAPs[i] *= fapu.IRISD;
		else if ( ( (i > 21) && (i < 26) ) || 
					( (i > 45) && (i < 50) ) )
			FAPs[i] *= fapu.AU;
		else if ( ( (i > 25) && (i < 28) ) || 
					( (i > 35) && (i < 40) ) )
			FAPs[i] *= fapu.ES;
		else if( ( (i > 29) && (i < 36) ) || 
					( (i > 39) && (i < 42) ) || (i > 59) )
			FAPs[i] *= fapu.ENS;
	}
}

void FAPFile::unscaleFAPVect(std::vector<float>& FAPs, const FAPU& fapu)
{
	for(size_t i = 0; i < 68; ++i)
	{
		if( ( (i > 1) && (i < 5) ) || 
			( (i > 6) && (i < 14)) ||
			( (i > 14)&& (i < 18)) ||
			(i == 43) || (i == 45) || (i == 50) || (i == 51) ||
			( (i > 53) && (i < 60)) )
		{
			FAPs[i] /= fapu.MNS;
		}
		else if ( (i == 5) || (i == 6) || (i == 14) || (i == 42) || 
			(i == 44) || (i == 52) || (i == 53) )
			FAPs[i] /= fapu.MW;
		else if ( ( (i > 17) && (i < 22) ) || (i == 28) || (i == 29))
			FAPs[i] /= fapu.IRISD;
		else if ( ( (i > 21) && (i < 26) ) || 
					( (i > 45) && (i < 50) ) )
			FAPs[i] /= fapu.AU;
		else if ( ( (i > 25) && (i < 28) ) || 
					( (i > 35) && (i < 40) ) )
			FAPs[i] /= fapu.ES;
		else if( ( (i > 29) && (i < 36) ) || 
					( (i > 39) && (i < 42) ) || (i > 59) )
			FAPs[i] /= fapu.ENS;
	}
}
/*!
	Advances the current FAP to the next one. If we are at the end of the stream,
	pointer to current FAP is set to 0, so getCurrentFAP() call following it
	will return 0. So this code is legal (althoug it does nothing but goes to the 
	end of the fap stream only);
	\code
	while(pFAPs->getCurrentFAP())
	{
		pFAPs->next();
	};
	\endcode
*/
void FAPFile::next()
{
	// clear if we are at the end (will cause crash somewhere probably, if not handled correctly)
	if (m_currentFAPID >= m_FAPs.size() - 1)
		m_currentFAP.clear();
	else // get the next one
		m_currentFAP = m_FAPs[++m_currentFAPID];
}

/*!
	Rewinds the stream to the first FAP in the stream.
*/
void FAPFile::rewind()
{
	m_currentFAPID = 0;
	m_currentFAP = m_FAPs[0];
}

/*!
	Saves a FAP stream to disk.
*/
bool FAPFile::save(IFapStream& faps, const FAPU& fapu, const std::string& filename)
{
	std::ofstream fap_file(filename.c_str());
	fap_file << faps.getVersion();
	fap_file << " Xface::FapFile ";
	fap_file << faps.getFPS();
	fap_file << " ";
	fap_file << (unsigned int)faps.getFAPCount() << std::endl;
	
	faps.rewind();
	std::vector<float> fap_set = faps.getCurrentFAP();
	int lineId = 0;
	while(!fap_set.empty())
	{
		unscaleFAPVect(fap_set, fapu);
		for (size_t i = 0; i < 68; ++i)
		{
			if(fap_set[i])
				fap_file << "1 ";
			else
				fap_file << "0 ";
		}
		fap_file << std::endl;

		fap_file << lineId++ << " ";
		for (size_t i = 0; i < 68; ++i)
		{
			if(fap_set[i])
				fap_file << fap_set[i] << " ";
		}
		fap_file << std::endl;
		
		faps.next();
		fap_set = faps.getCurrentFAP();
	}

	fap_file.close();
	return true;
}
} // namespace XFace