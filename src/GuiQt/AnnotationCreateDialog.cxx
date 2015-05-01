
/*LICENSE_START*/
/*
 *  Copyright (C) 2015 Washington University School of Medicine
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

#define __ANNOTATION_CREATE_DIALOG_DECLARE__
#include "AnnotationCreateDialog.h"
#undef __ANNOTATION_CREATE_DIALOG_DECLARE__

#include <QButtonGroup>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QToolButton>
#include <QVBoxLayout>

#include "AnnotationArrow.h"
#include "AnnotationBox.h"
#include "AnnotationCoordinate.h"
#include "AnnotationLine.h"
#include "AnnotationOval.h"
#include "AnnotationText.h"
#include "AnnotationFile.h"
#include "Brain.h"
#include "BrainOpenGLViewportContent.h"
#include "BrainOpenGLWidget.h"
#include "BrowserTabContent.h"
#include "CaretAssert.h"
#include "CaretDataFileSelectionComboBox.h"
#include "CaretDataFileSelectionModel.h"
#include "CaretFileDialog.h"
#include "CaretPointer.h"
#include "EventAnnotation.h"
#include "EventDataFileAdd.h"
#include "EventGraphicsUpdateOneWindow.h"
#include "EventManager.h"
#include "GuiManager.h"
#include "MouseEvent.h"
#include "SelectionItemSurfaceNode.h"
#include "SelectionItemVoxel.h"
#include "SelectionManager.h"
#include "Surface.h"
#include "WuQtUtilities.h"
#include "WuQMessageBox.h"

using namespace caret;


    
/**
 * \class caret::AnnotationCreateDialog 
 * \brief Dialog used for creating new annotations.
 * \ingroup GuiQt
 */

/**
 * Constructor.
 *
 * @param mouseEvent
 *     The mouse event indicating where user clicked in the window
 * @param annotationType
 *      Type of annotation that is being created.
 * @param parent
 *      Optional parent for this dialog.
 */
AnnotationCreateDialog::AnnotationCreateDialog(const MouseEvent& mouseEvent,
                                               const AnnotationTypeEnum::Enum annotationType,
                                               QWidget* parent)
: WuQDialogModal("Create Annotation",
                 parent),
m_annotationType(annotationType),
m_windowIndex(-1),
m_tabIndex(-1),
m_modelXYZValid(false)
{
    BrainOpenGLWidget* openGLWidget = mouseEvent.getOpenGLWidget();
    const int mouseX = mouseEvent.getX();
    const int mouseY = mouseEvent.getY();
    
    SelectionManager* idManager =
    openGLWidget->performIdentification(mouseX,
                                        mouseY,
                                        true);
    
    SelectionItemVoxel* voxelID = idManager->getVoxelIdentification();
    m_surfaceNodeIdentification = idManager->getSurfaceNodeIdentification();
    if (m_surfaceNodeIdentification->isValid()) {
        m_surfaceNodeIdentification->getModelXYZ(m_modelXYZ);
        m_modelXYZValid = true;
    }
    else if (voxelID->isValid()) {
        voxelID->getModelXYZ(m_modelXYZ);
        m_modelXYZValid = true;
    }
    
    BrainOpenGLViewportContent* vpContent = mouseEvent.getViewportContent();
    
    const int* tabViewport = vpContent->getModelViewport();
    m_tabXYZ[0] = mouseEvent.getX() - tabViewport[0];
    m_tabXYZ[1] = mouseEvent.getY() - tabViewport[1];
    m_tabXYZ[2] = 0.0;
    m_tabIndex  = vpContent->getBrowserTabContent()->getTabNumber();
    
    /*
     * Normalize tab coordinates (width and height range [0, 1]
     */
    m_tabXYZ[0] /= tabViewport[2];
    m_tabXYZ[1] /= tabViewport[3];
    
    const int* windowViewport = vpContent->getWindowViewport();
    m_windowXYZ[0] = windowViewport[0] + mouseEvent.getX(); // tabViewport[0] + tabViewport[2] + mouseEvent.getX();
    m_windowXYZ[1] = windowViewport[1] + mouseEvent.getY(); // tabViewport[1] + tabViewport[3] + mouseEvent.getY();
    m_windowXYZ[2] = 0.0;
    m_windowIndex = vpContent->getWindowIndex();
    
    /*
     * Normalize window coordinates (width and height range [0, 1]
     */
    m_windowXYZ[0] /= windowViewport[2];
    m_windowXYZ[1] /= windowViewport[3];
    
    std::cout << "Tab:    " << qPrintable(AString::fromNumbers(tabViewport, 4, ",")) << std::endl;
    std::cout << "Window: " << qPrintable(AString::fromNumbers(windowViewport, 4, ",")) << std::endl;
    
    m_fileSelectionWidget = createFileSelectionWidget();
    QWidget* spaceSelectionWidget = createSpaceSelectionWidget();

    QWidget* dialogWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(dialogWidget);
    if (m_fileSelectionWidget != NULL) {
        layout->addWidget(m_fileSelectionWidget);
    }
    layout->addWidget(spaceSelectionWidget);
    
    setCentralWidget(dialogWidget,
                     SCROLL_AREA_NEVER);
}

/**
 * Destructor.
 */
AnnotationCreateDialog::~AnnotationCreateDialog()
{
    delete m_annotationFileSelectionModel;
}

/**
 * @return New instance of the file selection widget.
 */
QWidget*
AnnotationCreateDialog::createFileSelectionWidget()
{
    QLabel* fileLabel = new QLabel("File: ");
    m_annotationFileSelectionModel = CaretDataFileSelectionModel::newInstanceForCaretDataFileType(GuiManager::get()->getBrain(),
                                                                                                  DataFileTypeEnum::ANNOTATION);
    if (s_previousAnnotationFile != NULL) {
        m_annotationFileSelectionModel->setSelectedFile(s_previousAnnotationFile);
    }
    m_annotationFileSelectionComboBox = new CaretDataFileSelectionComboBox(this);
    m_annotationFileSelectionComboBox->updateComboBox(m_annotationFileSelectionModel);
    
    QAction* newFileAction = WuQtUtilities::createAction("New",
                                                         "Create a new border file",
                                                         this,
                                                         this,
                                                         SLOT(newAnnotationFileButtonClicked()));
    QToolButton* newFileToolButton = new QToolButton();
    newFileToolButton->setDefaultAction(newFileAction);
    
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->addWidget(fileLabel, 0);
    layout->addWidget(m_annotationFileSelectionComboBox->getWidget(), 100);
    layout->addWidget(newFileToolButton);
    
    return widget;
}

/**
 * @return New instance of the space selection widget.
 */
QWidget*
AnnotationCreateDialog::createSpaceSelectionWidget()
{
    bool enableModelSpaceFlag   = false;
    bool enableSurfaceSpaceFlag = false;
    bool enableTabSpaceFlag     = true;
    bool enableWindowSpaceFlag  = true;
    
    switch (m_annotationType) {
        case AnnotationTypeEnum::ARROW:
            break;
        case AnnotationTypeEnum::BOX:
            enableModelSpaceFlag   = true;
            enableSurfaceSpaceFlag = true;
            break;
        case AnnotationTypeEnum::IMAGE:
            break;
        case AnnotationTypeEnum::LINE:
            break;
        case AnnotationTypeEnum::OVAL:
            enableModelSpaceFlag   = true;
            enableSurfaceSpaceFlag = true;
            break;
        case AnnotationTypeEnum::TEXT:
            enableModelSpaceFlag   = true;
            enableSurfaceSpaceFlag = true;
            break;
    }
    
    m_spaceButtonGroup = new QButtonGroup(this);
    
    int columnIndex = 0;
    const int COLUMN_RADIO_BUTTON = columnIndex++;
    const int COLUMN_COORD_X      = columnIndex++;
    const int COLUMN_COORD_Y      = columnIndex++;
    const int COLUMN_COORD_Z      = columnIndex++;
    const int COLUMN_EXTRA        = columnIndex++;
    
    QWidget* widget = new QWidget();
    QGridLayout* gridLayout = new QGridLayout(widget);
    gridLayout->setColumnStretch(COLUMN_RADIO_BUTTON, 0);
    gridLayout->setColumnStretch(COLUMN_COORD_X,      0);
    gridLayout->setColumnStretch(COLUMN_COORD_Y,      0);
    gridLayout->setColumnStretch(COLUMN_COORD_Z,      0);
    gridLayout->setColumnStretch(COLUMN_EXTRA,      100);
    
    const int titleRow = gridLayout->rowCount();
    gridLayout->addWidget(new QLabel("Space"),
                          titleRow, COLUMN_RADIO_BUTTON);
    gridLayout->addWidget(new QLabel("X"),
                          titleRow, COLUMN_COORD_X);
    gridLayout->addWidget(new QLabel("Y"),
                          titleRow, COLUMN_COORD_Y);
    gridLayout->addWidget(new QLabel("Z"),
                          titleRow, COLUMN_COORD_Z);
    
    if (m_modelXYZValid
        && enableModelSpaceFlag) {
        QRadioButton* rb = createRadioButtonForSpace(AnnotationCoordinateSpaceEnum::MODEL);
        m_spaceButtonGroup->addButton(rb,
                                      AnnotationCoordinateSpaceEnum::toIntegerCode(AnnotationCoordinateSpaceEnum::MODEL));
        
        const int rowNum = gridLayout->rowCount();
        gridLayout->addWidget(rb,
                              rowNum, COLUMN_RADIO_BUTTON);
        gridLayout->addWidget(new QLabel(AString::number(m_modelXYZ[0])),
                              rowNum, COLUMN_COORD_X);
        gridLayout->addWidget(new QLabel(AString::number(m_modelXYZ[1])),
                              rowNum, COLUMN_COORD_Y);
        gridLayout->addWidget(new QLabel(AString::number(m_modelXYZ[2])),
                              rowNum, COLUMN_COORD_Z);
        enableModelSpaceFlag = false;
    }
    
    if ((m_tabIndex >= 0)
        && enableTabSpaceFlag) {
        QRadioButton* rb = createRadioButtonForSpace(AnnotationCoordinateSpaceEnum::TAB);
        rb->setText(rb->text()
                    + " "
                    + AString::number(m_tabIndex + 1));
        m_spaceButtonGroup->addButton(rb,
                                      AnnotationCoordinateSpaceEnum::toIntegerCode(AnnotationCoordinateSpaceEnum::TAB));
        
        const int rowNum = gridLayout->rowCount();
        gridLayout->addWidget(rb,
                              rowNum, COLUMN_RADIO_BUTTON);
        gridLayout->addWidget(new QLabel(AString::number(m_tabXYZ[0])),
                              rowNum, COLUMN_COORD_X);
        gridLayout->addWidget(new QLabel(AString::number(m_tabXYZ[1])),
                              rowNum, COLUMN_COORD_Y);
        gridLayout->addWidget(new QLabel(AString::number(m_tabXYZ[2])),
                              rowNum, COLUMN_COORD_Z);
    }
    
    if ((m_windowIndex >= 0)
        && enableWindowSpaceFlag) {
        QRadioButton* rb = createRadioButtonForSpace(AnnotationCoordinateSpaceEnum::WINDOW);
        rb->setText(rb->text()
                    + " "
                    + AString::number(m_windowIndex + 1));
        m_spaceButtonGroup->addButton(rb,
                                      AnnotationCoordinateSpaceEnum::toIntegerCode(AnnotationCoordinateSpaceEnum::WINDOW));

        
        const int rowNum = gridLayout->rowCount();
        gridLayout->addWidget(rb,
                              rowNum, COLUMN_RADIO_BUTTON);
        gridLayout->addWidget(new QLabel(AString::number(m_windowXYZ[0])),
                              rowNum, COLUMN_COORD_X);
        gridLayout->addWidget(new QLabel(AString::number(m_windowXYZ[1])),
                              rowNum, COLUMN_COORD_Y);
        gridLayout->addWidget(new QLabel(AString::number(m_windowXYZ[2])),
                              rowNum, COLUMN_COORD_Z);
    }
    
    if (m_surfaceNodeIdentification->isValid()
        && enableSurfaceSpaceFlag) {
        
        QRadioButton* rb = createRadioButtonForSpace(AnnotationCoordinateSpaceEnum::SURFACE);
        m_spaceButtonGroup->addButton(rb,
                                      AnnotationCoordinateSpaceEnum::toIntegerCode(AnnotationCoordinateSpaceEnum::SURFACE));

        
        const int rowNum = gridLayout->rowCount();
        gridLayout->addWidget(rb,
                              rowNum, COLUMN_RADIO_BUTTON);
        const StructureEnum::Enum structure = m_surfaceNodeIdentification->getSurface()->getStructure();
        const AString infoText(StructureEnum::toGuiName(structure)
                               + " Vertex: "
                               +AString::number(m_surfaceNodeIdentification->getNodeNumber()));
        gridLayout->addWidget(new QLabel(infoText),
                              rowNum, COLUMN_COORD_X, 1, 4);
    }
    
    /*
     * This switch statment does nothing.  But, if a new space is added
     * the missing enumerated value in the switch statement will cause a
     * compilation error which may indicate the code in this method
     * needs to be updated.
     */
    const AnnotationCoordinateSpaceEnum::Enum space = AnnotationCoordinateSpaceEnum::TAB;
    switch (space) {
        case AnnotationCoordinateSpaceEnum::MODEL:
        case AnnotationCoordinateSpaceEnum::PIXELS:
        case AnnotationCoordinateSpaceEnum::SURFACE:
        case AnnotationCoordinateSpaceEnum::TAB:
        case AnnotationCoordinateSpaceEnum::WINDOW:
            break;
    }
    
    return widget;
}

/**
 * Create a radio button that displays the text for and contains the 
 * enumerated value in a property.
 */
QRadioButton*
AnnotationCreateDialog::createRadioButtonForSpace(const AnnotationCoordinateSpaceEnum::Enum space)
{
    const AString spaceGuiName = AnnotationCoordinateSpaceEnum::toGuiName(space);
    const QString spaceEnumName = AnnotationCoordinateSpaceEnum::toName(space);
    QRadioButton* rb = new QRadioButton(spaceGuiName);
    rb->setProperty(s_SPACE_PROPERTY_NAME.toAscii().constData(),
                    spaceEnumName);
    
    return rb;
}
/**
 * Gets called when "New" file button is clicked.
 */
void
AnnotationCreateDialog::newAnnotationFileButtonClicked()
{
    /*
     * Let user choose a different path/name
     */
    AnnotationFile* newFile = new AnnotationFile();
    AString newFileName = CaretFileDialog::getSaveFileNameDialog(DataFileTypeEnum::ANNOTATION,
                                                                       this,
                                                                       "Choose Annotation File Name",
                                                                       newFile->getFileName());
    /*
     * If user cancels, delete the new border file and return
     */
    if (newFileName.isEmpty()) {
        delete newFile;
        return;
    }
    
    /*
     * Set name of new border file, add file to brain, and make
     * file the selected annotation file.
     */
    newFile->setFileName(newFileName);
    EventManager::get()->sendEvent(EventDataFileAdd(newFile).getPointer());
    s_previousAnnotationFile = newFile;
    m_annotationFileSelectionModel->setSelectedFile(s_previousAnnotationFile);
    m_annotationFileSelectionComboBox->updateComboBox(m_annotationFileSelectionModel);
}

/**
 * Gets called when the OK button is clicked.
 */
void
AnnotationCreateDialog::okButtonClicked()
{
    AnnotationFile* annotationFile = m_annotationFileSelectionModel->getSelectedFileOfType<AnnotationFile>();
    if (annotationFile == NULL) {
        WuQMessageBox::errorOk(this, "An annotation file must be selected.");
        return;
    }
    
    const int checkedButtonID = m_spaceButtonGroup->checkedId();
    if (checkedButtonID < 0) {
        WuQMessageBox::errorOk(this, "A space must be selected.");
        return;
    }
    
    bool valid = false;
    const AnnotationCoordinateSpaceEnum::Enum space = AnnotationCoordinateSpaceEnum::fromIntegerCode(checkedButtonID,
                                                                                                     &valid);
    if ( ! valid) {
        CaretAssertMessage(0, "Space should have been valid.  This should not happen.");
        WuQMessageBox::errorOk(this, "A space must be selected.");
        return;
    }
    
    CaretPointer<Annotation> annotation;
    annotation.grabNew(NULL);
    
    switch (m_annotationType) {
        case AnnotationTypeEnum::ARROW:
            annotation.grabNew(new AnnotationArrow());
            break;
        case AnnotationTypeEnum::BOX:
            annotation.grabNew(new AnnotationBox());
            break;
        case AnnotationTypeEnum::IMAGE:
            break;
        case AnnotationTypeEnum::LINE:
            annotation.grabNew(new AnnotationLine());
            break;
        case AnnotationTypeEnum::OVAL:
            annotation.grabNew(new AnnotationOval());
            break;
        case AnnotationTypeEnum::TEXT:
        {
            AnnotationText* text = new AnnotationText();
            text->setText("NewText");
            annotation.grabNew(text);
        }
            break;
    }
    
    AnnotationTwoDimensionalShape* twoDimAnnotation = dynamic_cast<AnnotationTwoDimensionalShape*>(annotation.getPointer());
    AnnotationOneDimensionalShape* oneDimAnnotation = dynamic_cast<AnnotationOneDimensionalShape*>(annotation.getPointer());
    
    AnnotationCoordinate* firstCoordinate  = NULL;
    AnnotationCoordinate* secondCoordinate = NULL;
    
    if (twoDimAnnotation != NULL) {
        firstCoordinate = twoDimAnnotation->getCoordinate();
    }
    else if (oneDimAnnotation != NULL) {
        firstCoordinate = oneDimAnnotation->getStartCoordinate();
        secondCoordinate = oneDimAnnotation->getEndCoordinate();
    }

    switch (space) {
        case AnnotationCoordinateSpaceEnum::MODEL:
            if (m_modelXYZValid) {
                firstCoordinate->setXYZ(m_modelXYZ);
                annotation->setCoordinateSpace(AnnotationCoordinateSpaceEnum::MODEL);
            }
            break;
        case AnnotationCoordinateSpaceEnum::PIXELS:
            CaretAssert(0);
            break;
        case AnnotationCoordinateSpaceEnum::SURFACE:
            if (m_surfaceNodeIdentification->isValid()) {
                const Surface* surface = m_surfaceNodeIdentification->getSurface();
                firstCoordinate->setSurfaceSpace(surface->getStructure(),
                                                 surface->getNumberOfNodes(),
                                                 m_surfaceNodeIdentification->getNodeNumber());
                annotation->setCoordinateSpace(AnnotationCoordinateSpaceEnum::SURFACE);
            }
            break;
        case AnnotationCoordinateSpaceEnum::TAB:
            if (m_tabIndex >= 0) {
                firstCoordinate->setXYZ(m_tabXYZ);
                annotation->setCoordinateSpace(AnnotationCoordinateSpaceEnum::TAB);
                annotation->setTabIndex(m_tabIndex);
                
                if (secondCoordinate != NULL) {
                    double xyz[3] = {
                        m_tabXYZ[0],
                        m_tabXYZ[1],
                        m_tabXYZ[2]
                    };
                    if (xyz[1] > 0.5) {
                        xyz[1] -= 0.25;
                    }
                    else {
                        xyz[1] += 0.25;
                    }
                    secondCoordinate->setXYZ(xyz);
                }
            }
            break;
        case AnnotationCoordinateSpaceEnum::WINDOW:
            if (m_windowIndex >= 0) {
                firstCoordinate->setXYZ(m_windowXYZ);
                annotation->setCoordinateSpace(AnnotationCoordinateSpaceEnum::WINDOW);
                annotation->setWindowIndex(m_windowIndex);
                
                if (secondCoordinate != NULL) {
                    double xyz[3] = {
                        m_tabXYZ[0],
                        m_tabXYZ[1],
                        m_tabXYZ[2]
                    };
                    if (xyz[1] > 0.5) {
                        xyz[1] -= 0.25;
                    }
                    else {
                        xyz[1] += 0.25;
                    }
                    secondCoordinate->setXYZ(xyz);
                }
            }
            break;
    }
    
    /*
     * Need to release annotation from its CaretPointer since the
     * annotation file will take ownership of the annotation.
     */
    Annotation* annotationPointer = annotation.releasePointer();
    
    annotationFile->addAnnotation(annotationPointer);

    annotationPointer->setSelected(true);
    EventManager::get()->sendEvent(EventAnnotation().setModeEditAnnotation(m_windowIndex,
                                                                           annotationPointer).getPointer());
    EventManager::get()->sendEvent(EventGraphicsUpdateOneWindow(m_windowIndex).getPointer());
    
    WuQDialog::okButtonClicked();
}

