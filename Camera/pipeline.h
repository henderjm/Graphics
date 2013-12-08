/*
	Copyright 2010 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PIPELINE_H
#define	PIPELINE_H

#include "maths_funcs.h"

class Pipeline
{
public:
    Pipeline()
    {
        m_scale      = vec3(1.0f, 1.0f, 1.0f);
        m_worldPos   = vec3(0.0f, 0.0f, 0.0f);
        m_rotateInfo = vec3(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.v[0] = ScaleX;
        m_scale.v[1] = ScaleY;
        m_scale.v[2] = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        m_worldPos.v[0] = x;
        m_worldPos.v[1] = y;
        m_worldPos.v[2] = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo.v[0] = RotateX;
        m_rotateInfo.v[1] = RotateY;
        m_rotateInfo.v[2] = RotateZ;
    }

/*    void SetPerspectiveProj(const PersProjInfo& p)
    {
        m_persProjInfo = p;
    }
*/
    void SetCamera(const vec3& Pos, const vec3& Target, const vec3& Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }

    const mat4& GetWVPTrans();

    const mat4& GetWorldTrans();

private:
    vec3 m_scale;
    vec3 m_worldPos;
    vec3 m_rotateInfo;

//    PersProjInfo m_persProjInfo;

    struct {
        vec3 Pos;
        vec3 Target;
        vec3 Up;
    } m_camera;

    mat4 m_WVPtransformation;
    mat4 m_WorldTransformation;
};


#endif	/* PIPELINE_H */

