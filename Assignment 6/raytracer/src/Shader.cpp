/**
 * @file Shader.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for a 3D graphics shader.
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

#include "Shader.h"


/**
 * @brief Initializes the shader and its shading coefficients.
 */
Shader::Shader()
{
    ka = 0.0;
    kd = 0.0;
    ks = 0.0;
    n = 0.0;
}


/**
 * @brief Deinitializes the shader.
 */
Shader::~Shader()
{

}


/**
 * @brief Determines the color of an entity due to a particular light
 * in the 3D world.
 *
 * @param[in] light The light illuminating `entity`.
 *
 * @param[in] entity The entity we wish to color.
 *
 * @param[in] view The ray along which the entity is viewed.
 *
 * @param[in] to_light A ray from the entity at the point being 
 * viewed to the light `light`.
 *
 * @param[in] normal A surface normal at the surface of `entity`
 * at the point being viewed.
 *
 * @param[in,out] color On input, contains the current color of the 
 * pixel; the new color is added to the old one. This allows the
 * function to be called multiple times if there are multiple lights.
 */
void Shader::shade_from_single_light(Light *light, Entity *entity, Ray *view,
Ray *to_light, Ray *normal, Color *color)
{
    Ray *to_light_reflect, *to_viewer;
    Vertex *n, *l, v;
    float dot;

    float intensity = light->get_intensity_at_point(to_light->get_origin());
    Color *light_color = light->get_color();
    Color *entity_color = entity->get_material()->get_color();

    add_diffuse_color(intensity, light_color, entity_color, to_light,
        normal, color);

    dot = Ray::dot_product(normal, to_light);
    l = to_light->get_displacement();
    n = normal->get_displacement();

    v.x = 2.0 * dot * n->x - l->x;
    v.y = 2.0 * dot * n->y - l->y;
    v.z = 2.0 * dot * n->z - l->z;

    to_light_reflect = new Ray(new Vertex(to_light->get_origin()), new Vertex(&v));

    n = view->get_displacement();
    v.x = -n->x;
    v.y = -n->y;
    v.z = -n->z;

    to_viewer = new Ray(new Vertex(to_light->get_origin()), new Vertex(&v));

    add_specular_color(intensity, light_color, entity_color, to_viewer,
        to_light_reflect, color);

    delete to_light_reflect;
    delete to_viewer;
}


/**
 * @brief Calculates the diffuse lighting component (Lambertian) 
 * at a point.
 *
 * @attention `to_light` and `normal` should be pre-normalized.
 *
 * @param[in] intensity The light's intensity at the point in question.
 *
 * @param[in] light_color The light's color.
 *
 * @param[in] to_light A ray from the point to the light.
 *
 * @param[in] normal The surface normal at the point.
 *
 * @param[in,out] color On input, contains the current color of the 
 * pixel; the new color is added to the old one.
 */
void Shader::add_diffuse_color(float intensity, Color *light_color,
Color *entity_color, Ray *to_light, Ray *normal, Color *color)
{
    float dot = Ray::dot_product(to_light, normal);
    dot = 0.0 > dot ? 0.0 : dot;

    color->r += kd * intensity * light_color->r * entity_color->r * dot;
    color->g += kd * intensity * light_color->g * entity_color->g * dot;
    color->b += kd * intensity * light_color->b * entity_color->b * dot;
}


/**
 * @brief Calculates the specular lighting component (Blinn-Phong) 
 * at a point.
 *
 * @attention `to_viewer` and `reflect` should be pre-normalized.
 *
 * @param[in] intensity The light's intensity at the point in question.
 *
 * @param[in] light_color The light's color.
 *
 * @param[in] entity_color The color of the entity being lit.
 *
 * @param[in] to_viewer A ray from the point being viewed to the 
 * viewer.
 *
 * @param[in] reflect A ray along the light's reflection off the 
 * entity surface
 *
 * @param[in,out] color On input, contains the current color of the 
 * pixel; the new color is added to the old one.
 */
void Shader::add_specular_color(float intensity, Color *light_color,
Color *entity_color, Ray *to_viewer, Ray *reflect, Color *color)
{
    float dot = Ray::dot_product(to_viewer, reflect);
    dot = 0.0 > dot ? 0.0 : dot;

    color->r += intensity * light_color->r * entity_color->r * powf(dot, n);
    color->g += intensity * light_color->g * entity_color->g * powf(dot, n);
    color->b += intensity * light_color->b * entity_color->b * powf(dot, n);
}


/**
 * @brief Adds the ambient shading component to the input color
 * based on the ambient coefficient and the entity color.
 *
 * @param[in] entity The entity whose color is to be used.
 *
 * @param[in,out] color On input, contains the current color of the 
 * pixel; the new color is added to the old one.
 */
void Shader::shade_ambient(Entity *entity, Color *color)
{
   Color *entity_color = entity->get_material()->get_color();

   color->r += ka * entity_color->r;
   color->g += ka * entity_color->g;
   color->b += ka * entity_color->b;
}


/**
 * @brief Sets the internal ambient coefficient.
 *
 * @param[in] ka The new ambient coefficient.
 */
void Shader::set_ambient_coefficient(float ka)
{
    this->ka = ka;
}


/**
 * @brief Sets the internal diffuse coefficient.
 *
 * @param[in] kd The new diffuse coefficient.
 */
void Shader::set_diffuse_coefficient(float kd)
{
    this->kd = kd;
}


/**
 * @brief Sets the internal specular coefficients.
 *
 * @param[in] ks The new specular coefficient.
 *
 * @param[in] n The new specular exponent.
 */
void Shader::set_specular_coefficients(float ks, float n)
{
    this->ks = ks;
    this->n = n;
}

