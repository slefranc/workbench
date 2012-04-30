
/*LICENSE_START*/
/*
 * Copyright 2012 Washington University,
 * All rights reserved.
 *
 * Connectome DB and Connectome Workbench are part of the integrated Connectome 
 * Informatics Platform.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of Washington University nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR  
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*LICENSE_END*/

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QTabWidget>
#include <QToolButton>

#define __FOCI_SELECTION_VIEW_CONTROLLER_DECLARE__
#include "FociSelectionViewController.h"
#undef __FOCI_SELECTION_VIEW_CONTROLLER_DECLARE__

#include "Brain.h"
#include "BrainOpenGL.h"
#include "BrowserTabContent.h"
#include "CaretAssert.h"
#include "ClassAndNameHierarchyViewController.h"
#include "DisplayGroupEnumComboBox.h"
#include "DisplayPropertiesFoci.h"
#include "FociColoringTypeEnum.h"
#include "EventGraphicsUpdateAllWindows.h"
#include "EventManager.h"
#include "EventToolBoxUpdate.h"
#include "EventUserInterfaceUpdate.h"
#include "GuiManager.h"
#include "WuQDataEntryDialog.h"
#include "WuQtUtilities.h"

using namespace caret;


    
/**
 * \class caret::FociSelectionViewController 
 * \brief Widget for controlling display of foci
 *
 * Widget for controlling the display of foci including
 * different display groups.
 */

/**
 * Constructor.
 */
FociSelectionViewController::FociSelectionViewController(const int32_t browserWindowIndex,
                                             QWidget* parent)
: QWidget(parent)
{
    m_browserWindowIndex = browserWindowIndex;
    
    QWidget* attributesWidget = this->createAttributesWidget();
    QWidget* selectionWidget = this->createSelectionWidget();
    
    
    QTabWidget* tabWidget = new QTabWidget();
    tabWidget->addTab(attributesWidget, 
                      "Attributes");
    tabWidget->addTab(selectionWidget, 
                      "Selection");
    tabWidget->setCurrentWidget(attributesWidget);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget, 0, Qt::AlignLeft);
    layout->addStretch();
    
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_USER_INTERFACE_UPDATE);
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_TOOLBOX_UPDATE);
    
    FociSelectionViewController::allFociSelectionViewControllers.insert(this);
}

/**
 * Destructor.
 */
FociSelectionViewController::~FociSelectionViewController()
{
    EventManager::get()->removeAllEventsFromListener(this);
    
    FociSelectionViewController::allFociSelectionViewControllers.erase(this);
}


QWidget* 
FociSelectionViewController::createSelectionWidget()
{
    QLabel* groupLabel = new QLabel("Group");
    m_fociDisplayGroupComboBox = new DisplayGroupEnumComboBox(this);
    QObject::connect(m_fociDisplayGroupComboBox, SIGNAL(displayGroupSelected(const DisplayGroupEnum::Enum)),
                     this, SLOT(fociDisplayGroupSelected(const DisplayGroupEnum::Enum)));
    
    QHBoxLayout* groupLayout = new QHBoxLayout();
    groupLayout->addWidget(groupLabel);
    groupLayout->addWidget(m_fociDisplayGroupComboBox->getWidget());
    groupLayout->addStretch(); 
    
    m_fociClassNameHierarchyViewController = new ClassAndNameHierarchyViewController(m_browserWindowIndex);
    QObject::connect(m_fociClassNameHierarchyViewController, SIGNAL(itemSelected(ClassAndNameHierarchySelectedItem*)),
                     this, SLOT(fociSelectionsChanged(ClassAndNameHierarchySelectedItem*)));
    
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->addLayout(groupLayout);  
    layout->addWidget(m_fociClassNameHierarchyViewController);
    layout->addStretch();
    
    return widget;
}

/**
 * @return The attributes widget.
 */
QWidget* 
FociSelectionViewController::createAttributesWidget()
{
    m_fociDisplayCheckBox = new QCheckBox("Display Foci");
    QObject::connect(m_fociDisplayCheckBox, SIGNAL(clicked(bool)),
                     this, SLOT(processAttributesChanges()));
    
    m_fociContralateralCheckBox = new QCheckBox("Contralateral");
    QObject::connect(m_fociContralateralCheckBox, SIGNAL(clicked(bool)),
                     this, SLOT(processAttributesChanges()));
    
    std::vector<FociColoringTypeEnum::Enum> coloringTypeEnums;
    FociColoringTypeEnum::getAllEnums(coloringTypeEnums);
    const int32_t numDrawingTypeEnums = static_cast<int32_t>(coloringTypeEnums.size());
    
    QLabel* drawAsLabel = new QLabel("Coloring");
    m_coloringTypeComboBox = new QComboBox(); 
    for (int32_t i = 0; i < numDrawingTypeEnums; i++) {
        FociColoringTypeEnum::Enum drawType = coloringTypeEnums[i];
        m_coloringTypeComboBox->addItem(FociColoringTypeEnum::toGuiName(drawType),
                                    (int)drawType);
    }
    m_coloringTypeComboBox->setToolTip("Select the coloring assignment for foci");
    QObject::connect(m_coloringTypeComboBox, SIGNAL(activated(int)),
                     this, SLOT(processAttributesChanges()));
    
    float minLineWidth = 0;
    float maxLineWidth = 1000;
    //BrainOpenGL::getMinMaxLineWidth(minLineWidth,
    //                                maxLineWidth);
        
    QLabel* pointSizeLabel = new QLabel("Size");
    m_sizeSpinBox = new QDoubleSpinBox();
    m_sizeSpinBox->setFixedWidth(80);
    m_sizeSpinBox->setRange(minLineWidth,
                                 maxLineWidth);
    m_sizeSpinBox->setSingleStep(1.0);
    m_sizeSpinBox->setDecimals(1);
    m_sizeSpinBox->setToolTip("Adjust the size of foci");
    QObject::connect(m_sizeSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(processAttributesChanges()));
    
    
    QWidget* gridWidget = new QWidget();
    QGridLayout* gridLayout = new QGridLayout(gridWidget);
    WuQtUtilities::setLayoutMargins(gridLayout, 8, 2);
    int row = gridLayout->rowCount();
    gridLayout->addWidget(m_fociDisplayCheckBox, row, 0, 1, 2, Qt::AlignLeft);
    row++;
    gridLayout->addWidget(m_fociContralateralCheckBox, row, 0, 1, 2, Qt::AlignLeft);
    row++;
    gridLayout->addWidget(WuQtUtilities::createHorizontalLineWidget(), row, 0, 1, 2);
    row++;
    gridLayout->addWidget(drawAsLabel, row, 0);
    gridLayout->addWidget(m_coloringTypeComboBox, row, 1);
    row++;
    row++;
    gridLayout->addWidget(pointSizeLabel, row, 0);
    gridLayout->addWidget(m_sizeSpinBox, row, 1);
    gridWidget->setSizePolicy(QSizePolicy::Fixed,
                              QSizePolicy::Fixed);
    
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->addWidget(gridWidget);
    layout->addStretch();
        
    return widget;
}

/**
 * Called when a widget on the attributes page has 
 * its value changed.
 */
void 
FociSelectionViewController::processAttributesChanges()
{
    DisplayPropertiesFoci* dpf = GuiManager::get()->getBrain()->getDisplayPropertiesFoci();
    
    const int selectedColoringTypeIndex = m_coloringTypeComboBox->currentIndex();
    const int coloringTypeInteger = m_coloringTypeComboBox->itemData(selectedColoringTypeIndex).toInt();
    const FociColoringTypeEnum::Enum selectedColoringType = static_cast<FociColoringTypeEnum::Enum>(coloringTypeInteger);

    BrowserTabContent* browserTabContent = 
    GuiManager::get()->getBrowserTabContentForBrowserWindow(m_browserWindowIndex, true);
    if (browserTabContent == NULL) {
        return;
    }
    const int32_t browserTabIndex = browserTabContent->getTabNumber();
    dpf->setDisplayed(browserTabIndex,
                      m_fociDisplayCheckBox->isChecked());
    dpf->setContralateralDisplayed(browserTabIndex,
                                   m_fociContralateralCheckBox->isChecked());

    dpf->setColoringType(selectedColoringType);
    dpf->setFociSize(m_sizeSpinBox->value());
    
    EventManager::get()->sendEvent(EventGraphicsUpdateAllWindows().getPointer());
    
    updateOtherFociAttributesWidgets();
}

/**
 * Update the attributes widget
 */ 
void 
FociSelectionViewController::updateAttributesWidget()
{
    std::vector<FociColoringTypeEnum::Enum> coloringTypeEnums;
    FociColoringTypeEnum::getAllEnums(coloringTypeEnums);
    const int32_t numDrawingTypeEnums = static_cast<int32_t>(coloringTypeEnums.size());
    
    DisplayPropertiesFoci* dpf = GuiManager::get()->getBrain()->getDisplayPropertiesFoci();
    
    BrowserTabContent* browserTabContent = 
    GuiManager::get()->getBrowserTabContentForBrowserWindow(m_browserWindowIndex, true);
    if (browserTabContent == NULL) {
        return;
    }
    const int32_t browserTabIndex = browserTabContent->getTabNumber();
    
    m_fociDisplayCheckBox->setChecked(dpf->isDisplayed(browserTabIndex));
    m_fociContralateralCheckBox->setChecked(dpf->isContralateralDisplayed(browserTabIndex));
    
    const FociColoringTypeEnum::Enum selectedColoringType = dpf->getColoringType();
    int32_t selectedDrawingTypeIndex = 0;
    
    for (int32_t i = 0; i < numDrawingTypeEnums; i++) {
        FociColoringTypeEnum::Enum colorType = coloringTypeEnums[i];
        if (colorType == selectedColoringType) {
            selectedDrawingTypeIndex = i;
        }
    }
    m_coloringTypeComboBox->setCurrentIndex(selectedDrawingTypeIndex);
    
    m_sizeSpinBox->blockSignals(true);
    m_sizeSpinBox->setValue(dpf->getFociSize());
    m_sizeSpinBox->blockSignals(false);
}

/**
 * Called when the foci display group combo box is changed.
 */
void 
FociSelectionViewController::fociDisplayGroupSelected(const DisplayGroupEnum::Enum displayGroup)
{
    /*
     * Update selected display group in model.
     */
    BrowserTabContent* browserTabContent = 
    GuiManager::get()->getBrowserTabContentForBrowserWindow(m_browserWindowIndex, false);
    const int32_t browserTabIndex = browserTabContent->getTabNumber();
    Brain* brain = GuiManager::get()->getBrain();
    DisplayPropertiesFoci* dpf = brain->getDisplayPropertiesFoci();
    dpf->setDisplayGroup(browserTabIndex,
                         displayGroup);
    
    /*
     * Since display group has changed, need to update controls
     */
    updateFociSelectionViewController();
    
    /*
     * Apply the changes.
     */
    processFociSelectionChanges();
}

/**
 * Called when the foci selections are changed.
 * Updates foci display information and redraws
 * graphics.
 */
void 
FociSelectionViewController::fociSelectionsChanged(ClassAndNameHierarchySelectedItem* /*selectedItem*/)
{
    processFociSelectionChanges();
}

/**
 * Update the foci selection widget.
 */
void 
FociSelectionViewController::updateFociSelectionViewController()
{
    BrowserTabContent* browserTabContent = 
    GuiManager::get()->getBrowserTabContentForBrowserWindow(m_browserWindowIndex, true);
    if (browserTabContent == NULL) {
        return;
    }
    
    const int32_t browserTabIndex = browserTabContent->getTabNumber();
    
    setWindowTitle("Foci");
    
    Brain* brain = GuiManager::get()->getBrain();
    DisplayPropertiesFoci* dpf = brain->getDisplayPropertiesFoci();
    
    m_fociDisplayGroupComboBox->setSelectedDisplayGroup(dpf->getDisplayGroup(browserTabIndex));
    
    /*;
     * Get all of foci files.
     */
    std::vector<FociFile*> allFociFiles;
    const int32_t numberOfFociFiles = brain->getNumberOfFociFiles();
    for (int32_t iff = 0; iff < numberOfFociFiles; iff++) {
        allFociFiles.push_back(brain->getFociFile(iff));
    }
    
    /*
     * Update the class/name hierarchy
     */
    m_fociClassNameHierarchyViewController->updateContents(allFociFiles);
}

/**
 * Update other selection toolbox since they should all be the same.
 */
void 
FociSelectionViewController::updateOtherFociSelectionViewControllers()
{
    for (std::set<FociSelectionViewController*>::iterator iter = FociSelectionViewController::allFociSelectionViewControllers.begin();
         iter != FociSelectionViewController::allFociSelectionViewControllers.end();
         iter++) {
        FociSelectionViewController* bsw = *iter;
        if (bsw != this) {
            bsw->updateFociSelectionViewController();
        }
    }
}

/**
 * Update other foci attributes widgets since they all should be the same.
 */
void 
FociSelectionViewController::updateOtherFociAttributesWidgets()
{
    for (std::set<FociSelectionViewController*>::iterator iter = FociSelectionViewController::allFociSelectionViewControllers.begin();
         iter != FociSelectionViewController::allFociSelectionViewControllers.end();
         iter++) {
        FociSelectionViewController* bsw = *iter;
        if (bsw != this) {
            bsw->updateAttributesWidget();
        }
    }
}

/**
 * Gets called when foci selections are changed.
 */
void 
FociSelectionViewController::processFociSelectionChanges()
{
    BrowserTabContent* browserTabContent = 
    GuiManager::get()->getBrowserTabContentForBrowserWindow(m_browserWindowIndex, false);
    const int32_t browserTabIndex = browserTabContent->getTabNumber();
    Brain* brain = GuiManager::get()->getBrain();
    DisplayPropertiesFoci* dpf = brain->getDisplayPropertiesFoci();
    dpf->setDisplayGroup(browserTabIndex, 
                         m_fociDisplayGroupComboBox->getSelectedDisplayGroup());
    
    
    processSelectionChanges();
}

/**
 * Issue update events after selections are changed.
 */
void 
FociSelectionViewController::processSelectionChanges()
{
    updateOtherFociSelectionViewControllers();
    EventManager::get()->sendEvent(EventGraphicsUpdateAllWindows().getPointer());
}

/**
 * Receive events from the event manager.
 * 
 * @param event
 *   Event sent by event manager.
 */
void 
FociSelectionViewController::receiveEvent(Event* event)
{
    bool doUpdate = false;
    
    if (event->getEventType() == EventTypeEnum::EVENT_USER_INTERFACE_UPDATE) {
        EventUserInterfaceUpdate* uiEvent = dynamic_cast<EventUserInterfaceUpdate*>(event);
        CaretAssert(uiEvent);
        
        doUpdate = true;
        event->setEventProcessed();
    }
    else if (event->getEventType() == EventTypeEnum::EVENT_TOOLBOX_UPDATE) {
        EventToolBoxUpdate* tbEvent =
        dynamic_cast<EventToolBoxUpdate*>(event);
        if (tbEvent->isUpdateAllWindows()) {
            doUpdate = true;
        }
        else if (tbEvent->getBrowserWindowIndex() == m_browserWindowIndex) {
            doUpdate = true;
        }
        
        tbEvent->setEventProcessed();
    }

    if (doUpdate) {
        updateFociSelectionViewController();
        updateAttributesWidget();
    }
}


