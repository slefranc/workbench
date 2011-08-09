#ifndef __PALETTETHRESHOLDTYPE_H__
#define __PALETTETHRESHOLDTYPE_H__

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


#include <vector>
#include <QString>
#include <stdint.h>

namespace caret {

/**
 * Palette Threshold Type.
 */
class PaletteThresholdTypeEnum {

public:
    /**  Palette Threshold Type. */
    enum Enum {
        /** thresholding is off */
        THRESHOLD_TYPE_OFF,
        /** normal thresholding */
        THRESHOLD_TYPE_NORMAL,
        /** threshold from mapping of volume */
        THRESHOLD_TYPE_MAPPED,
        /** threshold from mapping to PALS average area */
        THRESHOLD_TYPE_MAPPED_AVERAGE_AREA
    };


    ~PaletteThresholdTypeEnum();

    static QString toName(Enum e);
    
    static Enum fromName(const QString& s, bool* isValidOut);
    
    static QString toGuiName(Enum e);
    
    static Enum fromGuiName(const QString& s, bool* isValidOut);
    
    static int32_t toIntegerCode(Enum e);
    
    static Enum fromIntegerCode(const int32_t integerCode, bool* isValidOut);

private:
    PaletteThresholdTypeEnum(const Enum e, const int32_t integerCode, const QString& name, const QString& guiName);

    static const PaletteThresholdTypeEnum* findData(const Enum e);
    
    static std::vector<PaletteThresholdTypeEnum> enumData;

    static void initialize();

    static bool initializedFlag;

    Enum e;

    int32_t integerCode;

    QString name;
    
    QString guiName;


};

#ifdef __PALETTE_THRESHOLDTYPE_DECLARE__
    std::vector<PaletteThresholdTypeEnum> PaletteThresholdTypeEnum::enumData;
bool PaletteThresholdTypeEnum::initializedFlag = false;
#endif // __PALETTE_THRESHOLDTYPE_DECLARE__

} // namespace

#endif // __PALETTETHRESHOLDTYPE_H__
