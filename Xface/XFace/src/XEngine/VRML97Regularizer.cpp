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
 * The Original Code is Xface Core Library; VRML97 Loader.
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.itc.it) Trento / ITALY.
 * For info, contact: xface-info@itc.it or http://xface.itc.it
 * Portions created by the Initial Developer are Copyright (C) 2004
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - John (johnson.tsing@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include <XEngine/VRML97Regularizer.h>

#include <string>
#include <strstream>
#include <fstream>
#include <sstream>

namespace XEngine{

    void VRML97Regularizer::regularize(const std::string &infilename, const std::string &outfilename)
    {
        std::ofstream ofs(outfilename.c_str(), std::ios::out|std::ios::binary);
        std::ifstream ifs(infilename.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
        if(ifs.fail() || ofs.fail())
            return;

        // load data to char*
        size_t size = (size_t)ifs.tellg();
        char* indata = new char[size];
        ifs.seekg(0, std::ios::beg);
        ifs.read(indata, size);
        ifs.close();

        // store to strstream
        std::stringstream instr(indata);
        std::streampos lstpos = 0;
        delete []indata;

        // parse input string,
        // store result to output string
        std::string outstr, keystr, chunk;
        while (instr >> keystr)
        {
            if (keystr == "IndexedFaceSet")
            {
                // store content till current keystr
                readTill(instr, lstpos, instr.tellg(), chunk);
                outstr += chunk;
                lstpos = instr.tellg();

                // process the indexed face set
                onIndexedFaceSet(instr, lstpos, outstr);
            }
        }
        // store content till eof
        instr.clear();
        readTill(instr, lstpos, size, chunk);
        outstr += chunk;

        // output result to file
        ofs.write(outstr.c_str(), outstr.length());
    }

    void VRML97Regularizer::readTill(std::istream& iStream, std::streampos begPos, std::streampos endPos, std::string& outStr) const
    {
        std::streampos curPos = iStream.tellg();
        iStream.seekg(begPos, std::ios::beg);
        size_t size = (size_t)(endPos-begPos);
        char* buf = new char[size];
        iStream.read(buf, size);
        outStr.clear();
        outStr.append(buf, size);
        delete []buf;
        iStream.seekg(curPos, std::ios::beg);
    }

    void VRML97Regularizer::readTill(std::istream& iStream, std::streampos begPos, const std::string& tgtStr, std::string& outStr) const
    {
        // read until target string
        std::string dummy;
        while (dummy != tgtStr)
            iStream >> dummy;

        // store content till target string
        readTill(iStream, begPos, iStream.tellg(), outStr);
    }

    void VRML97Regularizer::onIndexedFaceSet(std::istream& iStream, std::streampos& lstPos, std::string& outStr) const
    {
        std::vector<std::string> chunks;
        std::string achunk;

        std::vector<Vector3> vertices;
        std::vector<Vertex2D> texCoords;
        std::vector<Vector3> normals;
        std::vector< std::vector<unsigned short> > indices, indicesTex, indicesNorm;

        // read until "{"
        readTill(iStream, lstPos, "{", achunk);
        lstPos = iStream.tellg();
        chunks.push_back(achunk);

        // input from stream
        std::string dummy;
        short nBlock = 1;
        while (nBlock > 0)
        {
            iStream >> dummy;
            if (dummy == "{")
            {
                nBlock++;
            }
            else if (dummy == "}")
            {
                nBlock--;
            }
            else if(dummy == "coord")
            {
                // read until "["
                readTill(iStream, lstPos, "[", achunk);
                lstPos = iStream.tellg();
                chunks.push_back(achunk);

                // insert flag for "coord" points
                chunks.push_back("coord[]");

                // get "coord" values
                onCoord(iStream, vertices);
                lstPos = iStream.tellg();

                // read until "}"
                readTill(iStream, lstPos, "}", achunk);
                lstPos = iStream.tellg();
                chunks.push_back(achunk);
            }
            else if(dummy == "texCoord")
            {
                // read until "["
                readTill(iStream, lstPos, "[", achunk);
                lstPos = iStream.tellg();
                chunks.push_back(achunk);

                // insert flag for "texCoord" points
                chunks.push_back("texCoord[]");

                // get "texCoord" values
                onTexCoord(iStream, texCoords);
                lstPos = iStream.tellg();

                // read until "}"
                readTill(iStream, lstPos, "}", achunk);
                lstPos = iStream.tellg();
                chunks.push_back(achunk);
            }
            else if(dummy == "normal")
            {
                // read until "["
                readTill(iStream, lstPos, "[", achunk);
                lstPos = iStream.tellg();
                chunks.push_back(achunk);

                // insert flag for "normal" points
                chunks.push_back("normal[]");

                // get "normal" values
                onNormal(iStream, normals);
                lstPos = iStream.tellg();

                // read until "}"
                readTill(iStream, lstPos, "}", achunk);
                lstPos = iStream.tellg();
                chunks.push_back(achunk);
            }
            else if(dummy == "coordIndex")
            {
                // read until "["
                readTill(iStream, lstPos, "[", achunk);
                lstPos = iStream.tellg();
                chunks.push_back(achunk);

                // insert flag for "coordIndex" points
                chunks.push_back("coordIndex[]");

                // get "coordIndex" values
                onIndex(iStream, indices);
                lstPos = iStream.tellg();
            }
            else if(dummy == "texCoordIndex")
            {
                // read until "["
                readTill(iStream, lstPos, "[", achunk);
                lstPos = iStream.tellg();
                chunks.push_back(achunk);

                // insert flag for "texCoordIndex" points
                chunks.push_back("texCoordIndex[]");

                // get "texCoordIndex" values
                onIndex(iStream, indicesTex);
                lstPos = iStream.tellg();
            }
            else if(dummy == "normalIndex")
            {
                // read until "["
                readTill(iStream, lstPos, "[", achunk);
                lstPos = iStream.tellg();
                chunks.push_back(achunk);

                // insert flag for "normalIndex" points
                chunks.push_back("normalIndex[]");

                // get "normalIndex" values
                onIndex(iStream, indicesNorm, indices.size());
                lstPos = iStream.tellg();
            }
        }

        // read until "}"
        readTill(iStream, lstPos, iStream.tellg(), achunk);
        lstPos = iStream.tellg();
        chunks.push_back(achunk);

        // making texture coords indices exactly the same as vertex indices,
        // unfortunately, in VRML97 format, this isn't a rule, so we enforce it here
        if(!texCoords.empty())
        {
            // coords indices
            std::vector<Vector3> d_vertices = vertices;
            vertices.resize(texCoords.size());
            std::vector< std::vector<unsigned short> >::iterator it, it_t;
            std::vector<unsigned short>::iterator it2, it2_t;
            it_t = indicesTex.begin();
            for(it = indices.begin(); it != indices.end(); ++it, ++it_t)
            {
                it2_t = it_t->begin();
                for(it2 = it->begin(); it2 != it->end(); ++it2, ++it2_t)
                {
                    vertices[*it2_t] = d_vertices[*it2];
                }
            }
            // normal indices
            std::vector<Vector3> d_normals = normals;
            normals.resize(texCoords.size());
            it_t = indicesTex.begin();
            for(it = indicesNorm.begin(); it != indicesNorm.end(); ++it, ++it_t)
            {
                it2_t = it_t->begin();
                for(it2 = it->begin(); it2 != it->end(); ++it2, ++it2_t)
                {
                    normals[*it2_t] = d_normals[*it2];
                }
            }
        }
        indices = indicesTex;
        indicesNorm = indicesTex;

        // output regularized result
        for (std::vector<std::string>::iterator it = chunks.begin(); it != chunks.end(); ++it)
        {
            if (*it == "coord[]")
            {
                std::stringstream tmpstr;
                size_t cnt = 0;
                std::vector<Vector3>::const_iterator cit;
                for (cit = vertices.begin(); cit != vertices.end(); ++cit, ++cnt)
                {
                    if (cnt%3 == 0)
                        tmpstr << "\r\n          ";
                    tmpstr << cit->x << " " << cit->y << " " << cit->z << ", ";
                }
                outStr.append(tmpstr.str().c_str(), tmpstr.str().length()-2);
                outStr += "]";
            }
            else if (*it == "texCoord[]")
            {
                std::stringstream tmpstr;
                size_t cnt = 0;
                std::vector<Vertex2D>::const_iterator cit;
                for (cit = texCoords.begin(); cit != texCoords.end(); ++cit, ++cnt)
                {
                    if (cnt%4 == 0)
                        tmpstr << "\r\n          ";
                    tmpstr << cit->x << " " << cit->y << ", ";
                }
                outStr.append(tmpstr.str().c_str(), tmpstr.str().length()-2);
                outStr += "]";
            }
            else if (*it == "normal[]")
            {
                std::stringstream tmpstr;
                size_t cnt = 0;
                std::vector<Vector3>::const_iterator cit;
                for (cit = normals.begin(); cit != normals.end(); ++cit, ++cnt)
                {
                    if (cnt%3 == 0)
                        tmpstr << "\r\n          ";
                    tmpstr << cit->x << " " << cit->y << " " << cit->z << ", ";
                }
                outStr += tmpstr.str();
                outStr += "]";
            }
            else if (*it == "coordIndex[]")
            {
                std::stringstream tmpstr;
                size_t cnt = 0;
                std::vector< std::vector<unsigned short> >::iterator cit;
                std::vector<unsigned short>::iterator cit2;
                for (cit = indices.begin(); cit != indices.end(); ++cit, ++cnt)
                {
                    if (cnt%4 == 0)
                        tmpstr << "\r\n          ";
                    for (cit2 = cit->begin(); cit2 != cit->end(); ++cit2)
                        tmpstr << *cit2 << ", ";
                    tmpstr << "-1, ";
                }
                outStr.append(tmpstr.str().c_str(), tmpstr.str().length()-2);
                outStr += "]";
            }
            else if (*it == "texCoordIndex[]")
            {
                std::stringstream tmpstr;
                size_t cnt = 0;
                std::vector< std::vector<unsigned short> >::iterator cit;
                std::vector<unsigned short>::iterator cit2;
                for (cit = indicesTex.begin(); cit != indicesTex.end(); ++cit, ++cnt)
                {
                    if (cnt%4 == 0)
                        tmpstr << "\r\n          ";
                    for (cit2 = cit->begin(); cit2 != cit->end(); ++cit2)
                        tmpstr << *cit2 << ", ";
                    tmpstr << "-1, ";
                }
                outStr.append(tmpstr.str().c_str(), tmpstr.str().length()-2);
                outStr += "]";
            }
            else if (*it == "normalIndex[]")
            {
                std::stringstream tmpstr;
                size_t cnt = 0;
                std::vector< std::vector<unsigned short> >::iterator cit;
                std::vector<unsigned short>::iterator cit2;
                for (cit = indicesNorm.begin(); cit != indicesNorm.end(); ++cit, ++cnt)
                {
                    if (cnt%4 == 0)
                        tmpstr << "\r\n          ";
                    for (cit2 = cit->begin(); cit2 != cit->end(); ++cit2)
                        tmpstr << *cit2 << ", ";
                    tmpstr << "-1, ";
                }
                outStr += tmpstr.str();
                outStr += "]";
            }
            else
            {
                // output as is
                outStr += *it;
            }
        }
    }

    void VRML97Regularizer::onCoord(std::istream& pStream, std::vector<Vector3>& vertices) const
    {
        Vector3 coord3;
        std::string dummy;
        while (dummy != "]")
        {
            pStream >> coord3.x >> coord3.y >> coord3.z;
            vertices.push_back(coord3);
            pStream >> dummy; // should be comma (,) or end (])
        }
    }

    void VRML97Regularizer::onTexCoord(std::istream& pStream, std::vector<Vertex2D>& texCoords) const
    {
        Vertex2D coord2;
        std::string dummy;
        while (dummy != "]")
        {
            pStream >> coord2.x >> coord2.y;
            texCoords.push_back(coord2);
            pStream >> dummy; // should be comma (,) or end (])
        }
    }

    void VRML97Regularizer::onNormal(std::istream& pStream, std::vector<Vector3>& normals) const
    {
        Vector3 coord3;
        std::string dummy;
        pStream >> dummy;
        while(dummy != "]")
        {
            std::stringstream strStream(dummy);
            strStream >> coord3.x;
            pStream >> coord3.y >> coord3.z;
            normals.push_back(coord3);
            pStream >> dummy; // should be comma (,)
            pStream >> dummy; // should be end (]) or number
        }
    }

    void VRML97Regularizer::onIndex(std::istream& pStream, std::vector< std::vector<unsigned short> >& store, size_t number) const
    {
        long ind(0);
        std::vector<unsigned short> polyInd;
        std::string dummy;
        if (number == 0)
        {
            while (dummy != "]")
            {
                while (pStream >> ind >> dummy)
                {
                    if(ind == -1)
                        break;
                    polyInd.push_back((unsigned short)ind);
                }
                store.push_back(polyInd);
                polyInd.clear();
            }
        }
        else
        {
            for (size_t i=0; i<number; ++i)
            {
                while (pStream >> ind >> dummy)
                {
                    if(ind == -1)
                        break;
                    polyInd.push_back((unsigned short)ind);
                }
                store.push_back(polyInd);
                polyInd.clear();
            }
            while (dummy != "]")
                pStream >> dummy;
            return;
        }
    }

} // namespace XFace
