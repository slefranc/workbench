#ifndef __BROWSER_TAB_YOKING__H_
#define __BROWSER_TAB_YOKING__H_

/*LICENSE_START*/
/* 
 *  Copyright 1995-2011 Washington University School of Medicine 
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


#include "CaretObject.h"
#include "YokingTypeEnum.h"

namespace caret {

    class BrowserTabContent;
    
    class BrowserTabYoking : public CaretObject {

    public:
        BrowserTabYoking(BrowserTabContent* parentBrowserTabContent);
        
        virtual ~BrowserTabYoking();
        
        BrowserTabContent* getParentBrowserTabContent();
        
        void getYokableBrowserTabContent(std::vector<BrowserTabContent*>& yokableBrowserTabContentOut) const;
        
        BrowserTabContent* getYokedToBrowserTabContent();
        
        void setYokedToBrowserTabContent(BrowserTabContent* yokedToBrowserTabConent);
        
        YokingTypeEnum::Enum getYokingType() const;
        
        void setYokingType(YokingTypeEnum::Enum yokingType);
        
    private:
        BrowserTabYoking(const BrowserTabYoking&);

        BrowserTabYoking& operator=(const BrowserTabYoking&);
        
    public:
        virtual AString toString() const;
        
    private:
        BrowserTabContent* parentBrowserTabContent;
        
        BrowserTabContent* yokedToBrowserTabContent;
        
        YokingTypeEnum::Enum yokingType;
        
        static std::vector<BrowserTabYoking*> allBrowserTabYoking; 
    };
    
#ifdef __BROWSER_TAB_YOKING_DECLARE__
    std::vector<BrowserTabYoking*> BrowserTabYoking::allBrowserTabYoking;
#endif // __BROWSER_TAB_YOKING_DECLARE__

} // namespace
#endif  //__BROWSER_TAB_YOKING__H_
