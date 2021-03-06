
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

#define __BRAIN_OPEN_GL_SHAPE_DECLARE__
#include "BrainOpenGLShape.h"
#undef __BRAIN_OPEN_GL_SHAPE_DECLARE__

#include "BrainOpenGL.h"
#include "CaretAssert.h"
#include "CaretLogger.h"

using namespace caret;


    
/**
 * \class caret::BrainOpenGLShape 
 * \brief Abstract class for shapes drawn in OpenGL.
 *
 * Subclasses should allocate the 
 */

/**
 * Constructor.
 */
BrainOpenGLShape::BrainOpenGLShape()
: CaretObject()
{
    m_drawMode = BrainOpenGL::DRAW_MODE_INVALID;
    
    m_shapeSetupComplete = false;
}

/**
 * Destructor.
 */
BrainOpenGLShape::~BrainOpenGLShape()
{
    releaseOpenGLAllocations();
}

void
BrainOpenGLShape::releaseOpenGLAllocations()
{
    /*
     * Since 'releaseBufferIDInternal()' will alter 'm_bufferIDs'
     * make a copy of the set.  Otherwise, the iterator will get
     * corrupted and a crash will occur.
     */
    std::set<GLuint> bufferIDCopy = m_bufferIDs;
    for (std::set<GLuint>::iterator iter = bufferIDCopy.begin();
         iter != bufferIDCopy.end();
         iter++) {
        releaseBufferIDInternal(*iter);
    }
    m_bufferIDs.clear();
    
    /*
     * Since 'releaseDisplayListInternal()' will alter 'm_displayLists'
     * make a copy of the set.  Otherwise, the iterator will get
     * corrupted and a crash will occur.
     */
    std::set<GLuint> displayListsCopy = m_displayLists;
    for (std::set<GLuint>::iterator iter = displayListsCopy.begin();
         iter != displayListsCopy.end();
         iter++) {
        releaseDisplayListInternal(*iter);
    }
    m_displayLists.clear();    
}


/**
 * Force drawing mode to immediate mode since display lists
 * do not work during image capture.
 * @param override
 *    If true, immediate mode is always used until reset by
 *    calling this with false.
 */
void
BrainOpenGLShape::setImmediateModeOverride(const bool override)
{
    s_immediateModeOverride = override;
}

/**
 * Draw the shape.
 *
 * @param rgba
 *    Color for shape.
 */
void
BrainOpenGLShape::draw(const uint8_t rgba[4])
{
    createShapeIfNeeded();
    
    if (s_immediateModeOverride) {
        drawShape(BrainOpenGL::DRAW_MODE_IMMEDIATE,
                  rgba);
    }
    else {
        drawShape(m_drawMode,
                  rgba);
    }
}

/**
 * Create the shape or recreate it if drawing mode changed.
 */
void
BrainOpenGLShape::createShapeIfNeeded()
{
    if (m_drawMode == BrainOpenGL::DRAW_MODE_INVALID) {
        m_shapeSetupComplete = false;
    }
    if (m_drawMode != BrainOpenGL::getBestDrawingMode()) {
        m_shapeSetupComplete = false;
    }
    
    if (m_shapeSetupComplete == false) {
        releaseOpenGLAllocations();
        m_drawMode = BrainOpenGL::getBestDrawingMode();
        setupOpenGLForShape(m_drawMode);
        m_shapeSetupComplete = true;
    }
}

/**
 * Draw the shape.
 *
 * @param rgba
 *    Color for shape.
 */
void
BrainOpenGLShape::draw(const float rgba[4])
{
    createShapeIfNeeded();

    if (s_immediateModeOverride) {
        drawShape(BrainOpenGL::DRAW_MODE_IMMEDIATE,
                  rgba);
    }
    else {
        drawShape(m_drawMode,
                  rgba);
    }
}

/**
 * @return A new buffer ID for use with OpenGL.
 * A return value of zero indicates that creation of buffer ID failed.
 * Values greater than zero are valid buffer IDs.
 */
GLuint
BrainOpenGLShape::createBufferID()
{
    GLuint id = 0;
#ifdef BRAIN_OPENGL_INFO_SUPPORTS_VERTEX_BUFFERS
        glGenBuffers(1, &id);
        
        if (id > 0) {
            m_bufferIDs.insert(id);
        }
        else {
            CaretLogSevere("Failed to create a new OpenGL Vertex Buffer");
        }
#else // BRAIN_OPENGL_INFO_SUPPORTS_VERTEX_BUFFERS
    CaretLogSevere("PROGRAM ERROR: Creating OpenGL vertex buffer but vertex buffers not supported.");
#endif // BRAIN_OPENGL_INFO_SUPPORTS_VERTEX_BUFFERS
        
    return id;
}

/**
 * Release an allocated buffer ID.
 *
 * @param bufferID
 *    Buffer ID that was previously returned by createBufferID().
 */
void
BrainOpenGLShape::releaseBufferID(const GLuint bufferID)
{
    releaseBufferIDInternal(bufferID);
}

/**
 * Release an allocated buffer ID.
 *
 * @param bufferID
 *    Buffer ID that was previously returned by createBufferID().
 */
void
BrainOpenGLShape::releaseBufferIDInternal(const GLuint bufferID)
{
#ifdef BRAIN_OPENGL_INFO_SUPPORTS_VERTEX_BUFFERS
        if (glIsBuffer(bufferID)) {
            glDeleteBuffers(1, &bufferID);
            
            m_bufferIDs.erase(bufferID);
        }
        else {
            CaretLogSevere("PROGRAM ERROR: Attempting to delete invalid OpenGL BufferID="
                           + AString::number(bufferID));
        }
#else  // BRAIN_OPENGL_INFO_SUPPORTS_VERTEX_BUFFERS
    CaretLogSevere("PROGRAM ERROR: Releasing OpenGL vertex buffer #" + AString::number(bufferID) + " but vertex buffers not supported.");
#endif // BRAIN_OPENGL_INFO_SUPPORTS_VERTEX_BUFFERS
}

    
/**
 * Release an allocated buffer ID.
 *
 * @param bufferID
 *    Buffer ID that was previously returned by createBufferID().
 * @param isRemoveFromTrackedIDs
 *    If true, remove the ID from the bufferID that are tracked
 *    by this object.
 */
void
BrainOpenGLShape::releaseDisplayListInternal(const GLuint displayList)
{
#ifdef BRAIN_OPENGL_INFO_SUPPORTS_DISPLAY_LISTS
    if (BrainOpenGL::isDisplayListsSupported()) {
        if (glIsList(displayList)) {
            glDeleteLists(displayList, 1);
            
            m_displayLists.erase(displayList);
        }
        else {
            CaretLogSevere("PROGRAM ERROR: Attempting to delete invalid OpenGL DisplayList="
                           + AString::number(displayList));
        }
    }
#else  // BRAIN_OPENGL_INFO_SUPPORTS_DISPLAY_LISTS
    CaretLogSevere("PROGRAM ERROR: Releasing OpenGL display list #" + AString::number(displayList) + " but display lists not supported.");
#endif // BRAIN_OPENGL_INFO_SUPPORTS_DISPLAY_LISTS
}

/**
 * @return A new display list for use with OpenGL.
 * A return value of zero indicates that creation of display list failed.
 * Values greater than zero are valid display list.
 */
GLuint
BrainOpenGLShape::createDisplayList()
{
    GLuint displayList = 0;
#ifdef BRAIN_OPENGL_INFO_SUPPORTS_DISPLAY_LISTS
    if (BrainOpenGL::isDisplayListsSupported()) {
        displayList = glGenLists(1);
        
        if (displayList > 0) {
            m_displayLists.insert(displayList);
        }
        else {
            CaretLogSevere("Failed to create a new OpenGL Display List");
        }
    }
#else // BRAIN_OPENGL_INFO_SUPPORTS_DISPLAY_LISTS
    CaretLogSevere("PROGRAM ERROR: Creating OpenGL display list but display lists not supported.");
#endif // BRAIN_OPENGL_INFO_SUPPORTS_DISPLAY_LISTS
    
    return displayList;
}
    
/**
 * Release an allocated display list.
 *
 * @param displayList
 *    Display list that was previously returned by createDisplayList().
 */
void
BrainOpenGLShape::releaseDisplayList(const GLuint displayList)
{
    releaseDisplayListInternal(displayList);
}
 
/**
 * Convert a series of discontinuous triangle strips into one triangle strip.
 * Between each of the original triangle strips, additional vertices forming
 * degenerate (arealess) triangles are added.  This triangles are not drawn
 * by OpenGl.
 *
 * @see http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/concatenating-triangle-strips-r1871
 * @see http://en.wikipedia.org/wiki/Triangle_strip
 *
 * @param vertices
 *    The indices to all of the vertices in the input triangle strips.
 * @param stripStartIndices
 *    A vector containing the starting index of the strip in 'vertices'.
 * @param stripEndIndices
 *    A vector containing the ending index of the strip in 'vertices'.
 * @param triangleStripVerticesOut
 *    OUTPUT - A vector containing all of the vertex indices for drawing
 *    all of the input triangles with one triangle strip.
 */
void
BrainOpenGLShape::contatenateTriangleStrips(const std::vector<GLuint>& vertices,
                               const std::vector<GLuint>& stripStartIndices,
                               const std::vector<GLuint>& stripEndIndices,
                               std::vector<GLuint>& triangleStripVerticesOut) const
{
    triangleStripVerticesOut.clear();
    
    CaretAssert(stripStartIndices.size() == stripEndIndices.size());
    
    const int32_t numberOfStrips = static_cast<int32_t>(stripStartIndices.size());
    for (int32_t i = 0; i < numberOfStrips; i++) {
        const int32_t numInStrip = (stripEndIndices[i] - stripStartIndices[i]);
        if (numInStrip < 2) {
            continue;
        }        
        
        const int32_t startIndex = stripStartIndices[i];
        CaretAssertVectorIndex(vertices, startIndex);
        //const int32_t endIndex = stripEndIndices[i];
        //CaretAssertVectorIndex(vertices, endIndex);
        
        if (i > 0) {
            /*
             * N1 is index of first vertex in the strip
             * N2 is index of second vertex in the strip
             */
            const GLuint n1 = vertices[startIndex];
            CaretAssertVectorIndex(vertices, startIndex+1);
            const GLuint n2 = vertices[startIndex + 1];
            
            const GLuint prevEndIndex = stripEndIndices[i-1];
            const int32_t numInPrevStrip = (prevEndIndex - stripStartIndices[i-1]);
            if (numInPrevStrip >= 2) {
                /*
                 * P1 is index of last vertex in previous strip
                 * P2 is index of second to last vertex in previous strip
                 */
                CaretAssertVectorIndex(vertices, ((int32_t)prevEndIndex) - 1);
                const GLuint p1 = vertices[prevEndIndex - 1];
                CaretAssertVectorIndex(vertices, ((int32_t)prevEndIndex) - 2);
                const GLuint p2 = vertices[prevEndIndex - 2];
                
                /*
                 * Add degenerate triangles so that strips can be concatenated.
                 */
                triangleStripVerticesOut.push_back(p2);
                triangleStripVerticesOut.push_back(p1);
                triangleStripVerticesOut.push_back(p1);
                
                triangleStripVerticesOut.push_back(p1);
                triangleStripVerticesOut.push_back(p1);
                triangleStripVerticesOut.push_back(n1);
                
                triangleStripVerticesOut.push_back(p1);
                triangleStripVerticesOut.push_back(n1);
                triangleStripVerticesOut.push_back(n1);

                triangleStripVerticesOut.push_back(n1);
                triangleStripVerticesOut.push_back(n1);
                triangleStripVerticesOut.push_back(n2);
                
                triangleStripVerticesOut.push_back(n1);
                triangleStripVerticesOut.push_back(n2);
                triangleStripVerticesOut.push_back(n1);
                
                triangleStripVerticesOut.push_back(n1);
                triangleStripVerticesOut.push_back(n1);
                triangleStripVerticesOut.push_back(n2);
            }
        }
        
        const int32_t vertexStart = stripStartIndices[i];
        const int32_t vertexEnd = stripEndIndices[i];
        for (int32_t iVertex = vertexStart; iVertex < vertexEnd; iVertex++) {
            CaretAssertVectorIndex(vertices, iVertex);
            const int32_t vertexIndex = vertices[iVertex];
            triangleStripVerticesOut.push_back(vertexIndex);
        }
    }
}

/**
 * Convert a series of discontinuous triangle strips into one triangle strip.
 * Between each of the original triangle strips, additional vertices forming
 * degenerate (arealess) triangles are added.  This triangles are not drawn
 * by OpenGl.
 *
 * @see http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/concatenating-triangle-strips-r1871
 * @see http://en.wikipedia.org/wiki/Triangle_strip
 *
 * @param triangleStrips
 *    The triangle strips.  Each contains a sequence of vertex indices.
 * @param triangleStripOut
 *    OUTPUT - A vector containing all of the vertex indices for drawing
 *    all of the input triangles with one triangle strip.
 */
void
BrainOpenGLShape::contatenateTriangleStrips(const std::vector<std::vector<GLuint> >& triangleStrips,
                                            std::vector<GLuint>& triangleStripOut) const
{
    triangleStripOut.clear();
    
    const int32_t numberOfStrips = static_cast<int32_t>(triangleStrips.size());
    for (int32_t i = 0; i < numberOfStrips; i++) {
        const std::vector<GLuint>& strip = triangleStrips[i];
        const int32_t numInStrip = static_cast<int32_t>(strip.size());
        if (numInStrip < 2) {
            continue;
        }
        
        if (i > 0) {
            /*
             * N1 is index of first vertex in the strip
             * N2 is index of second vertex in the strip
             */
            CaretAssertVectorIndex(strip, 0);
            const GLuint n1 = strip[0];
            CaretAssertVectorIndex(strip, 1);
            const GLuint n2 = strip[1];
            
            
            const std::vector<GLuint>& prevStrip = triangleStrips[i - 1];
            const int32_t numInPrevStrip = static_cast<int32_t>(prevStrip.size());
            if (numInPrevStrip >= 2) {
                /*
                 * P1 is index of last vertex in previous strip
                 * P2 is index of second to last vertex in previous strip
                 */
                CaretAssertVectorIndex(prevStrip, numInPrevStrip - 1);
                const GLuint p1 = prevStrip[numInPrevStrip - 1];
                CaretAssertVectorIndex(prevStrip, numInPrevStrip - 2);
                const GLuint p2 = prevStrip[numInPrevStrip - 2];
                
                /*
                 * Add degenerate triangles so that strips can be concatenated.
                 */
                triangleStripOut.push_back(p2);
                triangleStripOut.push_back(p1);
                triangleStripOut.push_back(p1);
                
                triangleStripOut.push_back(p1);
                triangleStripOut.push_back(p1);
                triangleStripOut.push_back(n1);
                
                triangleStripOut.push_back(p1);
                triangleStripOut.push_back(n1);
                triangleStripOut.push_back(n1);
                
                triangleStripOut.push_back(n1);
                triangleStripOut.push_back(n1);
                triangleStripOut.push_back(n2);
                
                triangleStripOut.push_back(n1);
                triangleStripOut.push_back(n2);
                triangleStripOut.push_back(n1);
                
                triangleStripOut.push_back(n1);
                triangleStripOut.push_back(n1);
                triangleStripOut.push_back(n2);
            }
        }
        
        for (int32_t j = 0; j < numInStrip; j++) {
            triangleStripOut.push_back(strip[j]);
        }
    }
}

/**
 * Print the vertices in a triangle strip.  Each triplet is contained
 * within a set of parenthesis.
 *
 * @param triangleStrip
 *    Triangle strip that is printed.
 */
void
BrainOpenGLShape::printTriangleStrip(const std::vector<GLuint>& triangleStrip) const
{
    int i3 = 0;
    for (std::vector<GLuint>::const_iterator iter = triangleStrip.begin();
         iter != triangleStrip.end();
         iter++) {
        if (i3 == 0) {
            std::cout << " (";
        }
        else {
            std::cout << ",";
        }
        std::cout << *iter;
        if (i3 == 2) {
            std::cout << ")";
            i3 = 0;
        }
        else {
            i3++;
        }
    }
    std::cout << std::endl << std::endl;
}

/**
 * Print the vertices in a triangle fan
 *
 * @param triangleFan
 *    Triangle fan that is printed.
 */
void
BrainOpenGLShape::printTriangleFan(const std::vector<GLuint>& triangleFan) const
{
    const int32_t numVertices = static_cast<int32_t>(triangleFan.size());
    for (int32_t i = 0; i < numVertices; i++) {
        const GLuint vertexIndex = triangleFan[i];
        if (i == 0) {
            std::cout << "Center (";
        }
        else if (i > 1) {
            std::cout << ", ";
        }
        std::cout << vertexIndex;
        if (i == 0) {
            std::cout << ") Perimeter (";
        }        
    }
    if (numVertices > 1) {
        std::cout << ")";
    }
    std::cout << std::endl << std::endl;
}


