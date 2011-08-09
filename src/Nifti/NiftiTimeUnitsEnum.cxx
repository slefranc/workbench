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


#define __NIFTITIMEUNITS_DECLARE__
#include "NiftiTimeUnitsEnum.h"
#undef __NIFTITIMEUNITS_DECLARE__

#include <cassert>

using namespace caret;

/**
 * Constructor.
 *
 * @param e
 *    An enumerated value.
 * @param name
 *    Name of enumberated value.
 */
NiftiTimeUnitsEnum::NiftiTimeUnitsEnum(
                   const Enum e,
                   const int32_t integerCode,
                   const QString& name)
{
    this->e = e;
    this->name = name;
}

/**
 * Destructor.
 */
NiftiTimeUnitsEnum::~NiftiTimeUnitsEnum()
{
}

void
NiftiTimeUnitsEnum::initializeTimeUnits()
{
    if (initializedFlag) {
        return;
    }
    initializedFlag = true;

    enumData.push_back(NiftiTimeUnitsEnum(NIFTI_UNITS_UNKNOWN, 0,"NIFTI_UNITS_UNKNOWN"));
    enumData.push_back(NiftiTimeUnitsEnum(NIFTI_UNITS_SEC, 8,"NIFTI_UNITS_SEC"));
    enumData.push_back(NiftiTimeUnitsEnum(NIFTI_UNITS_MSEC, 16,"NIFTI_UNITS_MSEC"));
    enumData.push_back(NiftiTimeUnitsEnum(NIFTI_UNITS_USEC, 24,"NIFTI_UNITS_USEC"));
    enumData.push_back(NiftiTimeUnitsEnum(NIFTI_UNITS_HZ, 32,"NIFTI_UNITS_HZ"));
    enumData.push_back(NiftiTimeUnitsEnum(NIFTI_UNITS_PPM, 40,"NIFTI_UNITS_PPM"));
}

/**
 * Find the data for and enumerated value.
 * @param e
 *     The enumerated value.
 * @return Pointer to data for this enumerated type
 * or NULL if no data for type or if type is invalid.
 */
const NiftiTimeUnitsEnum*
NiftiTimeUnitsEnum::findData(const Enum e)
{
    initializeTimeUnits();
    int64_t num = enumData.size();
    for (int64_t i = 0; i < num; i++) {
        const NiftiTimeUnitsEnum* d = &enumData[i];
        if (d->e == e) {
            return d;
        }
    }
    assert(0);

    return NULL;
}

/**
 * Get a string representation of the enumerated type.
 * @param e 
 *     Enumerated value.
 * @param isValidOut 
 *     If not NULL, it is set indicating that a
 *     label exists for the input enum value.
 * @return 
 *     String representing enumerated value.
 */
QString 
NiftiTimeUnitsEnum::toName(Enum e) {
    initializeTimeUnits();
    
    const NiftiTimeUnitsEnum* ntu = findData(e);
    return ntu->name;
}

/**
 * Get an enumerated value corresponding to its name.
 * @param s 
 *     Name of enumerated value.
 * @param isValidOut 
 *     If not NULL, it is set indicating that a
 *     enum value exists for the input name.
 * @return 
 *     Enumerated value.
 */
NiftiTimeUnitsEnum::Enum 
NiftiTimeUnitsEnum::fromName(const QString& s, bool* isValidOut)
{
    initializeTimeUnits();
    
    bool validFlag = false;
    Enum e;
    
    for (std::vector<NiftiTimeUnitsEnum>::iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        const NiftiTimeUnitsEnum& d = *iter;
        if (d.name == s) {
            e = d.e;
            validFlag = true;
            break;
        }
    }
    
    if (isValidOut != 0) {
        *isValidOut = validFlag;
    }
    return e;
}
/**
 * Get the integer code associated with an time units.
 * @param e
 *   The enum.
 * @return 
 *   Integer code associated with time units.
 */
int32_t 
NiftiTimeUnitsEnum::toIntegerCode(Enum e)
{
    initializeTimeUnits();
    const NiftiTimeUnitsEnum* nsu = findData(e);
    return nsu->integerCode;
}

/**
 * Find enum corresponding to integer code.
 * @param integerCode
 *    The integer code.
 * @param isValidOut
 *    If not NULL, on exit it indicates valid integer code.
 * @return
 *    Enum corresponding to integer code.
 */
NiftiTimeUnitsEnum::Enum 
NiftiTimeUnitsEnum::fromIntegerCode(const int32_t integerCode, bool* isValidOut)
{
    initializeTimeUnits();
    
    bool validFlag = false;
    Enum e;
    
    for (std::vector<NiftiTimeUnitsEnum>::const_iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        const NiftiTimeUnitsEnum& nsu = *iter;
        if (nsu.integerCode == integerCode) {
            e = nsu.e;
            validFlag = true;
            break;
        }
    }
    
    if (isValidOut != 0) {
        *isValidOut = validFlag;
    }
    return e;
}


