/**
 * @file XMLSceneParser.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for an XML parser that constructs 3D scenes.
 * 
 * @section License
 * Copyright (c) 2013-2014 California Institute of Technology.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies, 
 * either expressed or implied, of the California Institute of Technology.
 * 
 */

#include "XMLSceneParser.h"


/**
 * @brief Initializes the parser.
 */
XMLSceneParser::XMLSceneParser()
{

}


/**
 * @brief Deinitializes the parser.
 */
XMLSceneParser::~XMLSceneParser()
{

}


/**
 * @brief Loads an XML-defined scene into a 3D world.
 *
 * @param[in] world The world that should contain the scene defined
 * in the input file.
 *
 * @param[in] file The XML file to load.
 *
 * @return 0 on no error, -1 otherwise.
 */
int XMLSceneParser::load_scene(World *world, char *file)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlChar *prop[10];

    float f1;
    Light *l;
    Material *m;
    Vertex *v1;
    Viewport *vp;

    /* Open the file for parsing. */
    doc = xmlParseFile(file);

    if (doc == NULL)
        return -1;

    /* Get the root element (scene). */
    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        xmlFreeDoc(doc);
        return -1;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) "scene") != 0)
    {
        xmlFreeDoc(doc);
        return -1;
    }

    /* Set the Viewport and Eye, and add the Entities and Lights to
     * the scene. */
    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) "eye") == 0)
        {
            prop[0] = xmlGetProp(cur, (const xmlChar *) "position");
            v1 = parse_vertex(prop[0]);
            xmlFree(prop[0]);

            world->set_eye(v1);
        }
        else if (xmlStrcmp(cur->name, (const xmlChar *) "viewport") == 0)
        {
            if ((vp = parse_viewport(cur->xmlChildrenNode)) == NULL)
            {
                return -1;
            }
            else
            {
                world->set_viewport(vp);
            }
        }
        else if (xmlStrcmp(cur->name, (const xmlChar *) "sphere") == 0)
        {
            prop[0] = xmlGetProp(cur, (const xmlChar *) "origin");
            prop[1] = xmlGetProp(cur, (const xmlChar *) "radius");

            v1 = parse_vertex(prop[0]);
            f1 = parse_float(prop[1]);
            m = parse_material(cur->xmlChildrenNode);

            xmlFree(prop[0]);
            xmlFree(prop[1]);

            world->add_entity((Entity *) new Sphere(v1, f1, m));
        }
        else if (xmlStrcmp(cur->name, (const xmlChar *) "light") == 0)
        {
            prop[0] = xmlGetProp(cur, (const xmlChar *) "type");

            if (xmlStrcmp(prop[0], (const xmlChar *) "point") == 0)
            {
                l = parse_pointlight(cur);
            }

            xmlFree(prop[0]);
            world->add_light(l);
        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);
    return 0;
}


/**
 * @brief Parses an xmlChar string of the form "(%f, %f, %f)" to a new
 * corresponding Color.
 *
 * @param[in] str The string to parse.
 *
 * @return The color equivalent to the input string. If a NULL string 
 * is supplied, it returns a default (white).
 */
Color *XMLSceneParser::parse_color(xmlChar *str)
{
    float r, g, b;

    if (str == NULL)
    {
        return new Color(1.0, 1.0, 1.0);
    }
    else
    {
        sscanf((char *) str, "(%f, %f, %f)", &r, &g, &b);
        return new Color(r, g, b);
    }
}


/**
 * @brief Parses an `xmlChar` string to a float.
 *
 * @param[in] str The string to parse.
 *
 * @return A float corresponding to the string. If a NULL string is 
 * supplied, it returns a default (zero).
 */
float XMLSceneParser::parse_float(xmlChar *str)
{
    float f;

    if (str == NULL)
    {
        return 0.0;
    }
    else
    {
        sscanf((char *) str, "%f", &f);
        return f;
    }
}


/**
 * @brief Parses a material node.
 *
 * @attention Note: This function will always return a valid Material. 
 * If an error occurs, it will return a default (white matte) material.
 *
 * @param[in] node A pointer to a node defining a material.
 *
 * @return A valid material.
 */
Material *XMLSceneParser::parse_material(xmlNodePtr node)
{
    xmlNodePtr cur = node;
    xmlChar *prop[10];

    Color *c;
    float f1, f2;

    while (cur != NULL)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) "material") == 0)
        {
            prop[0] = xmlGetProp(cur, (const xmlChar *) "color");
            prop[1] = xmlGetProp(cur, (const xmlChar *) "reflectivity");
            prop[2] = xmlGetProp(cur, (const xmlChar *) "refractivity");

            c = parse_color(prop[0]);
            f1 = parse_float(prop[1]);
            f2 = parse_float(prop[2]);

            xmlFree(prop[0]);
            xmlFree(prop[1]);
            xmlFree(prop[2]);

            return new Material(c, f1, f2);
        }

        cur = cur->next;
    }

    return new Material(new Color(1.0, 1.0, 1.0), 0.0, 0.0);
}


/**
 * @brief Parses a point light node.
 *
 * @param[in] node A pointer to a node defining a point light.
 *
 * @return A valid point light.
 */
Light *XMLSceneParser::parse_pointlight(xmlNodePtr node)
{
    Color *c;
    float f1, f2, f3, f4;
    Vertex *v;

    xmlChar *prop[6];

    prop[0] = xmlGetProp(node, (const xmlChar *) "position");
    prop[1] = xmlGetProp(node, (const xmlChar *) "intensity");
    prop[2] = xmlGetProp(node, (const xmlChar *) "color");
    prop[3] = xmlGetProp(node, (const xmlChar *) "att0");
    prop[4] = xmlGetProp(node, (const xmlChar *) "att1");
    prop[5] = xmlGetProp(node, (const xmlChar *) "att2");

    v = parse_vertex(prop[0]);
    f1 = parse_float(prop[1]);
    c = parse_color(prop[2]);
    f2 = parse_float(prop[3]);
    f3 = parse_float(prop[4]);
    f4 = parse_float(prop[5]);

    xmlFree(prop[0]);
    xmlFree(prop[1]);
    xmlFree(prop[2]);
    xmlFree(prop[3]);
    xmlFree(prop[4]);
    xmlFree(prop[5]);

    return (Light *) (new PointLight(v, f1, c, f2, f3, f4));
}


/**
 * @brief Parses an `xmlChar` string of the form "(%f, %f, %f)" to a new
 * corresponding Vertex.
 *
 * @param[in] str The string to parse.
 *
 * @return The vertex equivalent to the input string. If a NULL string 
 * is supplied, it returns a default (the origin).
 */
Vertex *XMLSceneParser::parse_vertex(xmlChar *str)
{
    float x, y, z;

    if (str == NULL)
    {
        return new Vertex(0.0, 0.0, 0.0);
    }
    else
    {
        sscanf((char *) str, "(%f, %f, %f)", &x, &y, &z);
        return new Vertex(x, y, z);
    }
}


/**
 * @brief Parses a viewport node.
 *
 * @param[in] node A pointer to a node defining a viewport.
 *
 * @return Returning a valid viewport if no error occurs, NULL otherwise.
 */
Viewport *XMLSceneParser::parse_viewport(xmlNodePtr node)
{
    xmlNodePtr cur = node;
    xmlChar *prop;
    Vertex *topleft = NULL, *topright = NULL, *bottomleft = NULL;

    while (cur != NULL)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) "topleft") == 0)
        {
            prop = xmlGetProp(cur, (const xmlChar *) "position");
            topleft = parse_vertex(prop);
            xmlFree(prop);
        }
        else if (xmlStrcmp(cur->name, (const xmlChar *) "topright") == 0)
        {
            prop = xmlGetProp(cur, (const xmlChar *) "position");
            topright = parse_vertex(prop);
            xmlFree(prop);
        }
        else if (xmlStrcmp(cur->name, (const xmlChar *) "bottomleft") == 0)
        {
            prop = xmlGetProp(cur, (const xmlChar *) "position");
            bottomleft = parse_vertex(prop);
            xmlFree(prop);
        }

        cur = cur->next;
    }

    if ((topleft == NULL) || (topright == NULL) || (bottomleft == NULL))
    {
        return NULL;
    }
    else
    {
        return new Viewport(topleft, topright, bottomleft);
    }
}

