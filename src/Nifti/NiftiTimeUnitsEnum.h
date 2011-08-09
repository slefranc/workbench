#ifndef __NIFTITIMEUNITS_H__
#define __NIFTITIMEUNITS_H__

/*LICENSE_START*/ 
/* 
 *  Copyright 1995-2002 Washington University School of Medicine 
 * 
 *  http://brainmap.wustl.edu 
 * 
 *  This file is part of CARET. 
 * 
 *  CARET is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version. 
 * 
 *  CARET is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 * 
 *  You should have received a copy of the GNU General Public License 
 *  along with CARET; if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 */ 



#include <stdint.h>

#include <vector>
#include <QString>

namespace caret {

/**
 * NIFTI Time Units
 */
class NiftiTimeUnitsEnum {

public:
    /**  NIFTI Time Units
 */
    enum Enum {
        /**  */
        NIFTI_UNITS_UNKNOWN,
        /**  */
        NIFTI_UNITS_SEC,
        /**  */
        NIFTI_UNITS_MSEC,
        /**  */
        NIFTI_UNITS_USEC,
        /**  */
        NIFTI_UNITS_HZ,
        /**  */
        NIFTI_UNITS_PPM
    };


    ~NiftiTimeUnitsEnum();

    static QString toName(Enum e);
    
    static Enum fromName(const QString& s, bool* isValidOut);
    
    static int32_t toIntegerCode(Enum e);

    static Enum fromIntegerCode(const int32_t integerCode, bool* isValidOut);

private:
    NiftiTimeUnitsEnum(const Enum e, const int32_t integerCode, const QString& name);

    static const NiftiTimeUnitsEnum* findData(const Enum e);
    
    static std::vector<NiftiTimeUnitsEnum> enumData;

    static void initializeTimeUnits();

    static bool initializedFlag;

    Enum e;

    int32_t integerCode;
    
    QString name;


};

#ifdef __NIFTITIMEUNITS_DECLARE__
    std::vector<NiftiTimeUnitsEnum> NiftiTimeUnitsEnum::enumData;
    bool NiftiTimeUnitsEnum::initializedFlag = false;
#endif // __NIFTITIMEUNITS_DECLARE__

} // namespace

#endif // __NIFTITIMEUNITS_H__
