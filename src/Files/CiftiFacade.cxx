
/*LICENSE_START*/
/*
 * Copyright 2013 Washington University,
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

#define __CIFTI_FACADE_DECLARE__
#include "CiftiFacade.h"
#undef __CIFTI_FACADE_DECLARE__

#include "CaretAssert.h"
#include "CiftiInterface.h"

using namespace caret;


    
/**
 * \class caret::CiftiFacade 
 * \brief A simpler interface (facade pattern) for CIFTI Interface access.
 * \ingroup Files
 */

/**
 * Constructor.
 *
 * @param ciftiInterface
 *    The CIFTI Interface.
 */
CiftiFacade::CiftiFacade(const DataFileTypeEnum::Enum dataFileType,
                         CiftiInterface* ciftiInterface)
: CaretObject(),
m_dataFileType(dataFileType),
m_ciftiInterface(ciftiInterface),
m_ciftiFileType(CIFTI_INVALID),
m_validCiftiFile(false)
{
    CaretAssert(ciftiInterface);
    
    switch (dataFileType) {
        case DataFileTypeEnum::BORDER:
            break;
        case DataFileTypeEnum::CONNECTIVITY_DENSE:
            m_ciftiFileType = CIFTI_DENSE;
            break;
        case DataFileTypeEnum::CONNECTIVITY_DENSE_LABEL:
            m_ciftiFileType = CIFTI_LABEL;
            break;
        case DataFileTypeEnum::CONNECTIVITY_DENSE_PARCEL:
            m_ciftiFileType = CIFTI_DENSE_PARCEL;
            break;
        case DataFileTypeEnum::CONNECTIVITY_DENSE_SCALAR:
            m_ciftiFileType = CIFTI_SCALAR;
            break;
        case DataFileTypeEnum::CONNECTIVITY_DENSE_TIME_SERIES:
            m_ciftiFileType = CIFTI_DATA_SERIES;
            break;
        case DataFileTypeEnum::CONNECTIVITY_FIBER_ORIENTATIONS_TEMPORARY:
            break;
        case DataFileTypeEnum::CONNECTIVITY_FIBER_TRAJECTORY_TEMPORARY:
            break;
        case DataFileTypeEnum::CONNECTIVITY_PARCEL:
            m_ciftiFileType = CIFTI_PARCEL;
            break;
        case DataFileTypeEnum::CONNECTIVITY_PARCEL_DENSE:
            m_ciftiFileType = CIFTI_PARCEL_DENSE;
            break;
        case DataFileTypeEnum::FOCI:
            break;
        case DataFileTypeEnum::LABEL:
            break;
        case DataFileTypeEnum::METRIC:
            break;
        case DataFileTypeEnum::PALETTE:
            break;
        case DataFileTypeEnum::RGBA:
            break;
        case DataFileTypeEnum::SCENE:
            break;
        case DataFileTypeEnum::SPECIFICATION:
            break;
        case DataFileTypeEnum::SURFACE:
            break;
        case DataFileTypeEnum::VOLUME:
            break;
        case DataFileTypeEnum::UNKNOWN:
            break;
    }
    
    m_numberOfColumns = m_ciftiInterface->getNumberOfColumns();
    m_numberOfRows    = m_ciftiInterface->getNumberOfRows();
    m_numberOfMaps    = 0;
    
    m_connectivityMatrixFileFlag = false;
    m_useColumnMapsForBrainordinateMapping = false;
    m_useRowMapsForBrainordinateMapping = false;
    m_useAlongRowMethodsForMapAttributes = false;
    m_loadBrainordinateDataFromColumns = false;
    m_loadBrainordinateDataFromRows = false;
    m_brainordinateDataColoredWithPalette = false;
    m_brainordinateDataColoredWithLabelTable = false;
    m_useParcelsForBrainordinateMapping = false;
    
    switch (m_ciftiFileType) {
        case CIFTI_INVALID:
            break;
        case CIFTI_DENSE:
            m_connectivityMatrixFileFlag = true;
            m_useRowMapsForBrainordinateMapping = true;
            m_numberOfMaps = 1;
            m_loadBrainordinateDataFromRows = true;
            m_brainordinateDataColoredWithPalette = true;
            break;
        case CIFTI_LABEL:
            m_useColumnMapsForBrainordinateMapping = true;
            m_useAlongRowMethodsForMapAttributes = true;
            m_loadBrainordinateDataFromColumns = true;
            m_numberOfMaps = m_numberOfColumns;
            m_brainordinateDataColoredWithLabelTable = true;
            break;
        case CIFTI_SCALAR:
            m_useColumnMapsForBrainordinateMapping = true;
            m_useAlongRowMethodsForMapAttributes = true;
            m_loadBrainordinateDataFromColumns = true;
            m_numberOfMaps = m_numberOfColumns;
            m_brainordinateDataColoredWithPalette = true;
            break;
        case CIFTI_DENSE_PARCEL:
            m_connectivityMatrixFileFlag = true;
            m_useRowMapsForBrainordinateMapping = true;
            m_numberOfMaps = 1;
            m_loadBrainordinateDataFromRows = true;
            m_brainordinateDataColoredWithPalette = true;
            m_useParcelsForBrainordinateMapping = true;
            break;
        case CIFTI_DATA_SERIES:
            m_useColumnMapsForBrainordinateMapping = true;
            m_useAlongRowMethodsForMapAttributes = true;
            m_loadBrainordinateDataFromColumns = true;
            m_numberOfMaps = m_numberOfColumns;
            m_brainordinateDataColoredWithPalette = true;
            break;
        case CIFTI_PARCEL:
            m_connectivityMatrixFileFlag = true;
            m_useRowMapsForBrainordinateMapping = true;
            m_numberOfMaps = 1;
            m_loadBrainordinateDataFromRows = true;
            m_brainordinateDataColoredWithPalette = true;
            m_useParcelsForBrainordinateMapping = true;
            break;
        case CIFTI_PARCEL_DENSE:
            m_connectivityMatrixFileFlag = true;
            m_useRowMapsForBrainordinateMapping = true;
            m_numberOfMaps = 1;
            m_loadBrainordinateDataFromRows = true;
            m_brainordinateDataColoredWithPalette = true;
            break;
    }
    
    CaretAssertMessage((m_ciftiFileType != CIFTI_INVALID),
                       ("Support for "
                        + DataFileTypeEnum::toName(m_dataFileType)
                        + " needs to be implemented or is invalid type."));
    
    if (m_ciftiFileType != CIFTI_INVALID) {
        m_validCiftiFile = true;
    }

}

/**
 * Destructor.
 */
CiftiFacade::~CiftiFacade()
{
    
}

/**
 * @return True if the file is a valid CIFTI file, else false.
 */
bool
CiftiFacade::isValidCiftiFile() const
{
    return m_validCiftiFile;
}


/**
 * @return The number of rows.
 */
int32_t
CiftiFacade::getNumberOfRows() const
{
    return m_numberOfRows;
}

/**
 * @return The number of columns.
 */
int32_t
CiftiFacade::getNumberOfColumns() const
{
    return m_numberOfColumns;
}

/**
 * @return The number of maps.
 */
int32_t
CiftiFacade::getNumberOfMaps() const
{
    return m_numberOfMaps;
}

/**
 * Return the number of elements contained in ONE map.
 */
int32_t
CiftiFacade::getMapDataCount() const
{
    int32_t count = 0;
    
    if (m_loadBrainordinateDataFromColumns) {
        count = m_numberOfRows;
    }
    else if (m_loadBrainordinateDataFromRows) {
        count = m_numberOfColumns;
    }
    else {
        CaretAssert(0);
    }
    
    return count;
}


/**
 * @return True if the interface is for a connectivity matrix file,
 *         else false.
 */
bool
CiftiFacade::isConnectivityMatrixFile() const
{
    return m_connectivityMatrixFileFlag;
}

/**
 * Get the metadata for the file.
 *
 * @param metadataOut
 *    Is cleared and metadata for the map in placed into this.
 */
void
CiftiFacade::getFileMetadata(GiftiMetaData* metadataOut)
{
    CaretAssert(metadataOut);
    metadataOut->clear();

    metadataOut->replaceWithMap(*m_ciftiInterface->getCiftiXML().getFileMetaData());
}

void
CiftiFacade::setFileMetadata(GiftiMetaData* metadataIn)
{
    CaretAssert(metadataIn);
    
    const CiftiXML& ciftiXML = m_ciftiInterface->getCiftiXML();
    *(ciftiXML.getFileMetaData()) = metadataIn->getAsMap();
}

/**
 * Get the mapping for the surface with the given structure for mapping
 * to brainordinates such as surface coloring or obtaining data values.
 *
 * @param mappingOut
 *    Output surface mapping.
 * @param structure
 *    Structure for the surface.
 * @return
 *    True if mapping is valid, else false.
 */
bool
CiftiFacade::getSurfaceMapForMappingDataToBrainordinates(std::vector<CiftiSurfaceMap>& mappingOut,
                                                         const StructureEnum::Enum structure) const
{
    mappingOut.clear();
    
    bool validFlag = false;
    if (m_useColumnMapsForBrainordinateMapping) {
        validFlag = m_ciftiInterface->getSurfaceMapForColumns(mappingOut,
                                                              structure);
    }
    else if (m_useRowMapsForBrainordinateMapping) {
        validFlag = m_ciftiInterface->getSurfaceMapForRows(mappingOut,
                                                           structure);
    }
    else {
        CaretAssert(0);
    }
    
    return validFlag;
}

/** 
 * Get the mapping for voxels  for mapping to brainordinates such as 
 * voxel coloring or obtaining data values.
 *
 * @param mappingOut
 *    Output surface mapping.
 * @param structure
 *    Structure for the surface.
 * @return
 *    True if mapping is valid, else false.
 */
bool
CiftiFacade::getVolumeMapForMappingDataToBrainordinates(std::vector<CiftiVolumeMap>& mappingOut) const
{
    mappingOut.clear();
    
    bool validFlag = false;
    if (m_useColumnMapsForBrainordinateMapping) {
        validFlag = m_ciftiInterface->getVolumeMapForColumns(mappingOut);
    }
    else if (m_useRowMapsForBrainordinateMapping) {
        validFlag = m_ciftiInterface->getVolumeMapForRows(mappingOut);
    }
    else {
        CaretAssert(0);
    }
    
    return validFlag;
}
/**
 * Get the parcels for mapping to brainordinates.
 *
 * @param parcelsOut
 *    Output containing parcels.
 * @return True if parcels are valid, else false.
 */
bool
CiftiFacade::getParcelMapForMappingToBrainordinates(std::vector<CiftiParcelElement>& parcelsOut) const
{
    bool validFlag = false;
    
    if (m_useColumnMapsForBrainordinateMapping) {
        validFlag = m_ciftiInterface->getCiftiXML().getParcelsForColumns(parcelsOut);
    }
    else if (m_useRowMapsForBrainordinateMapping) {
        validFlag = m_ciftiInterface->getCiftiXML().getParcelsForRows(parcelsOut);
    }
    
    return validFlag;
}

///**
// * Get the Parcel Nodes Element for the surface with the given structure.
// */
//bool
//CiftiFacade::getParcelSurfaceMapForMappingToBrainordinates(CiftiParcelNodesElement& nodeParcelsOut,
//                                                   const StructureEnum::Enum structure) const
//{
//    nodeParcelsOut.m_structure = StructureEnum::StructureEnum::INVALID;
//    nodeParcelsOut.m_nodes.clear();
//    
//    std::vector<CiftiParcelElement> parcels;
//    getParcelMapForMappingToBrainordinates(parcels);
//    
//    /*
//     * Match to structure.
//     */
//    for (std::vector<CiftiParcelElement>::iterator iter = parcels.begin();
//         iter != parcels.end();
//         iter++) {
//        CiftiParcelElement& cpe = *iter;
//        std::vector<CiftiParcelNodesElement> parcelNodes = cpe.m_nodeElements;
//        for (std::vector<CiftiParcelNodesElement>::iterator nodesIter = parcelNodes.begin();
//             nodesIter != parcelNodes.end();
//             nodesIter++) {
//            CiftiParcelNodesElement& cpne = *nodesIter;
//            if (cpne.m_structure == structure) {
//                nodeParcelsOut = cpne;
//            }
//            if (nodeParcelsOut.m_nodes.empty() == false) {
//                return true;
//            }
//        }
//    }
//    return false;
//}

/**
 * @return True if brainordinate mapping uses parcels.
 */
bool
CiftiFacade::isMappingDataToBrainordinateParcels() const
{
    return m_useParcelsForBrainordinateMapping;
}

/**
 * Get the indices into the data for the surface nodes of the given structure.
 * Works for brainordinates mapped a surfaces or parcels.
 *
 * @param dataIndicesForNodes
 *    Upon exit, value 'i' contains an index into the data for node 'i'.
 *    If the value 'i' is negative, there is no data for the node 'i'.
 * @param structure
 *    Surface's structure.
 * @param surfaceNumberOfNodes
 *    Number of nodes in the surface.
 */
bool
CiftiFacade::getSurfaceDataIndicesForMappingToBrainordinates(std::vector<int64_t>& dataIndicesForNodes,
                                                             const StructureEnum::Enum structure,
                                                    const int64_t surfaceNumberOfNodes) const
{
    dataIndicesForNodes.resize(surfaceNumberOfNodes,
                               -1);
    
    
    if (m_useParcelsForBrainordinateMapping) {
        for (int64_t i = 0; i < surfaceNumberOfNodes; i++) {
            int64_t dataIndex = -1;
            
            if (m_useColumnMapsForBrainordinateMapping) {
                dataIndex = m_ciftiInterface->getCiftiXML().getColumnParcelForNode(i,
                                                                                  structure);
            }
            else if (m_useRowMapsForBrainordinateMapping) {
                dataIndex = m_ciftiInterface->getCiftiXML().getRowParcelForNode(i,
                                                                                structure);
            }
            else {
                CaretAssert(0);
            }
            
            dataIndicesForNodes[i] = dataIndex;
        }
    }
    else {
        std::vector<CiftiSurfaceMap> surfaceMaps;
        if (getSurfaceMapForMappingDataToBrainordinates(surfaceMaps,
                                                        structure)) {
            const int64_t numSurfaceMaps = static_cast<int64_t>(surfaceMaps.size());
            for (int64_t i = 0; i < numSurfaceMaps; i++) {
                const CiftiSurfaceMap& csm = surfaceMaps[i];
                CaretAssert((csm.m_surfaceNode >= 0)
                            && (csm.m_surfaceNode < surfaceNumberOfNodes));
                dataIndicesForNodes[csm.m_surfaceNode] = csm.m_ciftiIndex;
            }
        }
    }
    return false;
}

/**
 * @return True if there is data for mapping to any surface.
 */
bool
CiftiFacade::containsSurfaceDataForMappingToBrainordinates() const
{
    std::vector<CiftiSurfaceMap> mapping;
    
    /*
     * Check for surface node mappings
     */
    std::vector<StructureEnum::Enum> allStructures;
    StructureEnum::getAllEnums(allStructures);
    for (std::vector<StructureEnum::Enum>::iterator iter = allStructures.begin();
         iter != allStructures.end();
         iter++) {
        getSurfaceMapForMappingDataToBrainordinates(mapping,
                                                    *iter);
            
        if (mapping.empty() == false) {
            return true;
        }
    }
    
    /*
     * Check for parcel node mappings.
     */
    std::vector<CiftiParcelElement> parcels;
    getParcelMapForMappingToBrainordinates(parcels);
    for (std::vector<CiftiParcelElement>::iterator iter = parcels.begin();
         iter != parcels.end();
         iter++) {
        const CiftiParcelElement& cpe = *iter;
        const std::vector<CiftiParcelNodesElement> parcelNodes = cpe.m_nodeElements;
        
        for (std::vector<CiftiParcelNodesElement>::const_iterator nodesIter = parcelNodes.begin();
             nodesIter != parcelNodes.end();
             nodesIter++) {
            const CiftiParcelNodesElement& cpne = *nodesIter;
            if (cpne.m_nodes.empty() == false) {
                return true;
            }
        }
    }
    
    return false;
}

/**
 * @return True if there is data for mapping to any volume.
 */
bool
CiftiFacade::containsVolumeDataForMappingToBrainordinates() const
{
    std::vector<CiftiVolumeMap> mapping;
    getVolumeMapForMappingDataToBrainordinates(mapping);
    
    if (mapping.empty()) {
        return false;
    }
    
    /*
     * Check for parcel node mappings.
     */
    std::vector<CiftiParcelElement> parcels;
    getParcelMapForMappingToBrainordinates(parcels);
    for (std::vector<CiftiParcelElement>::iterator iter = parcels.begin();
         iter != parcels.end();
         iter++) {
        const CiftiParcelElement& cpe = *iter;
        
        if (cpe.m_voxelIndicesIJK.empty() == false) {
            return true;
        }
    }
    
    return true;
}

/**
 * Get the metadata for the given map or series index.
 *
 * @param mapIndex
 *    Index of map or series.
 * @param metadataOut
 *    Is cleared and metadata for the map in placed into this.
 * @return
 *    True if the map index is valid (even if there is no metadata).
 */
bool
CiftiFacade::getMetadataForMapOrSeriesIndex(const int32_t mapIndex,
                                    GiftiMetaData* metadataOut)
{
    CaretAssert((mapIndex >= 0) && (mapIndex < m_numberOfMaps));
    CaretAssert(metadataOut);
    metadataOut->clear();
    
    if (m_connectivityMatrixFileFlag) {
        getFileMetadata(metadataOut);
        return true;
    }
    else if (m_useAlongRowMethodsForMapAttributes) {
        metadataOut->replaceWithMap(*m_ciftiInterface->getCiftiXML().getMapMetadata(CiftiXML::ALONG_ROW,
                                                                                    mapIndex));
        return true;
    }
    else {
        CaretAssert(0);
    }
    
    return false;
}

/**
 * Set the metadata for the given map or series index.
 *
 * @param mapIndex
 *    Index of map or series.
 * @param metadataOut
 *    Metadata that is placed using the given map or series index.
 */
void
CiftiFacade::setMetadataForMapOrSeriesIndex(const int32_t mapIndex,
                                    GiftiMetaData* metadataIn)
{
    CaretAssert((mapIndex >= 0) && (mapIndex < m_numberOfMaps));
    CaretAssert(metadataIn);
    
    if (m_connectivityMatrixFileFlag) {
        setFileMetadata(metadataIn);
    }
    else if (m_useAlongRowMethodsForMapAttributes) {
        const CiftiXML& ciftiXML = m_ciftiInterface->getCiftiXML();
        *(ciftiXML.getMapMetadata(CiftiXML::ALONG_ROW,
                                  mapIndex)) = metadataIn->getAsMap();
    }
    else {
        CaretAssert(0);
    }
}

/**
 * Get the label table for the given map or series index.
 * 
 * @param mapIndex
 *    Index of map or series.
 * @return
 *    Pointer to label table or NULL if label table not valid for 
 *    the given map or series index.
 */
GiftiLabelTable*
CiftiFacade::getLabelTableForMapOrSeriesIndex(const int32_t mapIndex)
{
    CaretAssert((mapIndex >= 0) && (mapIndex < m_numberOfMaps));
    GiftiLabelTable* labelTable = NULL;
    
    if (m_useAlongRowMethodsForMapAttributes) {
        labelTable = m_ciftiInterface->getCiftiXML().getLabelTableForRowIndex(mapIndex);
    }
    else {
        /* Not all files have label table. */
    }
    
    return labelTable;
}

/**
 * Get the palette color mapping for the given map or series index.
 *
 * @param mapIndex
 *    Index of map or series.
 * @return
 *    Pointer to palette color mapping or NULL if palette color mapping
 *    not valid for the given map or series index.
 */
PaletteColorMapping*
CiftiFacade::getPaletteColorMappingForMapOrSeriesIndex(const int32_t mapIndex)
{
    CaretAssert((mapIndex >= 0) && (mapIndex < m_numberOfMaps));
    PaletteColorMapping* pcm = NULL;
    
    if (m_connectivityMatrixFileFlag) {
        pcm = m_ciftiInterface->getCiftiXML().getFilePalette();
    }
    else if (m_useAlongRowMethodsForMapAttributes) {
        pcm = m_ciftiInterface->getCiftiXML().getMapPalette(CiftiXML::ALONG_ROW,
                                                            mapIndex);
    }
    else {
        /* Not all files have palette mapping. */
    }
    
    return pcm;
}

/**
 * Get the name for the given map or series index.
 *
 * @param mapIndex
 *    Index of map or series.
 * @return
 *    Name for given map or series index.  
 */
AString
CiftiFacade::getNameForMapOrSeriesIndex(const int32_t mapIndex) const
{
    CaretAssert((mapIndex >= 0) && (mapIndex < m_numberOfMaps));
    AString name;
    
    if (m_useAlongRowMethodsForMapAttributes) {
        name = m_ciftiInterface->getCiftiXML().getMapNameForRowIndex(mapIndex);
    }
    else if (m_connectivityMatrixFileFlag) {
        name = m_ciftiInterface->getCiftiXML().getMapNameForColumnIndex(mapIndex);
        /* no name for connectivity matrix */
    }
    else {
        CaretAssert(0);
    }
    
    return name;
}

/**
 * Set the name for the given map or series index.
 *
 * @param mapIndex
 *    Index of map or series.
 * @param name
 *    Name for the given map or series index.
 */
void
CiftiFacade::setNameForMapOrSeriesIndex(const int32_t mapIndex,
                                const AString name)
{
    CaretAssert((mapIndex >= 0) && (mapIndex < m_numberOfMaps));
    const CiftiXML& ciftiXML = m_ciftiInterface->getCiftiXML();
    
    if (m_useAlongRowMethodsForMapAttributes) {
        ciftiXML.setMapNameForRowIndex(mapIndex,
                                       name);
    }
    else if (m_connectivityMatrixFileFlag) {
        ciftiXML.setMapNameForColumnIndex(mapIndex,
                                         name);
    }
    else {
        CaretAssert(0);
    }
}

/**
 * Get the data for the given map or series index.
 *
 * @param mapIndex
 *    Index of map or series.
 * @param dataOut
 *    Data for given map or series is loaded into this.
 * @return
 *    True if output data is valid, else false.
 */
bool
CiftiFacade::getDataForMapOrSeriesIndex(const int32_t mapIndex,
                                        std::vector<float>& dataOut) const
{
    CaretAssert((mapIndex >= 0) && (mapIndex < m_numberOfMaps));
    if (m_loadBrainordinateDataFromColumns) {
        dataOut.resize(m_numberOfRows);
        m_ciftiInterface->getColumn(&dataOut[0],
                                    mapIndex);
    }
    else if (m_loadBrainordinateDataFromRows) {
        dataOut.resize(m_numberOfColumns);
        m_ciftiInterface->getRow(&dataOut[0],
                                 mapIndex);
    }
    else {
        CaretAssert(0);
    }
    
    return false;
}

/**
 * @return Is the brainordinate data mapped with a palette?
 */
bool
CiftiFacade::isBrainordinateDataColoredWithPalette() const
{
    return m_brainordinateDataColoredWithPalette;
}

/**
 * @return Is the brainordinate data mapped with a label table?
 */
bool
CiftiFacade::isBrainordinateDataColoredWithLabelTable() const
{
    return m_brainordinateDataColoredWithLabelTable;
}


