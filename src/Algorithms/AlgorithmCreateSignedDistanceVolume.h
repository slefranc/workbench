#ifndef __ALGORITHM_CREATE_SIGNED_DISTANCE_VOLUME_H__
#define __ALGORITHM_CREATE_SIGNED_DISTANCE_VOLUME_H__

/*LICENSE_START*/
/*
 *  Copyright (C) 2014  Washington University School of Medicine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*LICENSE_END*/

#include "AbstractAlgorithm.h"
#include "Vector3D.h"
#include "CaretMutex.h"
#include "OctTree.h"
#include "SignedDistanceHelper.h"

#include <vector>

namespace caret {

    struct DistVoxOffset
    {
        float m_dist;
        int m_offset[3];
    };
    
    struct VoxelIndex
    {//needed in order to contain the data in a heap, operator= doesn't work on a static array
        VoxelIndex(int64_t ijk[3])
        {
            m_ijk[0] = ijk[0];
            m_ijk[1] = ijk[1];
            m_ijk[2] = ijk[2];
        }
        int64_t m_ijk[3];
    };
    
    class AlgorithmCreateSignedDistanceVolume : public AbstractAlgorithm
    {
        AlgorithmCreateSignedDistanceVolume();
    protected:
        static float getSubAlgorithmWeight();
        static float getAlgorithmInternalWeight();
    public:
        AlgorithmCreateSignedDistanceVolume(ProgressObject* myProgObj, const SurfaceFile* mySurf, VolumeFile* myVolOut, VolumeFile* myRoiOut = NULL, const float& fillValue = 0.0f, const float& exactLim = 5.0f,
                                            const float& approxLim = 20.0f, const int& approxNeighborhood = 2, const SignedDistanceHelper::WindingLogic& myWinding = SignedDistanceHelper::EVEN_ODD);
        static OperationParameters* getParameters();
        static void useParameters(OperationParameters* myParams, ProgressObject* myProgObj);
        static AString getCommandSwitch();
        static AString getShortDescription();
    };
    
    typedef TemplateAutoOperation<AlgorithmCreateSignedDistanceVolume> AutoAlgorithmCreateSignedDistanceVolume;

}

#endif //__ALGORITHM_CREATE_SIGNED_DISTANCE_VOLUME_H__
