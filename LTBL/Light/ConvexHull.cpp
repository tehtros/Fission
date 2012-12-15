/*
	Let There Be Light
	Copyright (C) 2012 Eric Laukien

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
		claim that you wrote the original software. If you use this software
		in a product, an acknowledgment in the product documentation would be
		appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
		misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
*/

#include <LTBL/Light/ConvexHull.h>
#include <LTBL/Utils.h>

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

namespace ltbl
{
	ConvexHull::ConvexHull()
		: m_worldCenter(0.0f, 0.0f),
		m_aabbCalculated(false),
		m_updateRequired(true), // Remains true permanently unless user purposely changes it
		m_transparency(1.0f),
		m_renderLightOverHull(true)
	{
	}

	void ConvexHull::CenterHull()
	{
		// Calculate the average of all of the vertices, and then
		// offset all of them to make this the new origin position (0,0)
		const unsigned int numVertices = m_vertices.size();

		Vec2f posSum(0.0f, 0.0f);

		for(unsigned int i = 0; i < numVertices; i++)
			posSum += m_vertices[i];

		Vec2f averagePos(posSum / static_cast<float>(numVertices));

		for(unsigned int i = 0; i < numVertices; i++)
			m_vertices[i] -= averagePos;
	}

	bool ConvexHull::LoadShape(const char* fileName, float ptu)
	{
		std::ifstream load(fileName);

		if(!load)
		{
			load.close();

			std::cout << "Could not load convex hull \"" << fileName << "\"!" << std::endl;

			return false;
		}
		else
		{
		    int index = 0;
		    int indices;
		    load >> indices;

			while(index < indices)
			{
				float x, y;

				load >> x >> y;

                m_origVertices.push_back(Vec2f(x*ptu, y*ptu));
				m_vertices.push_back(Vec2f(x*ptu, y*ptu));
				index++;
			}

			load.close();
		}

		//CenterHull();

		CalculateNormals();

		return true;
	}

	Vec2f ConvexHull::GetWorldVertex(unsigned int index) const
	{
		assert(index >= 0 && index < m_vertices.size());
		return Vec2f(m_vertices[index].x + m_worldCenter.x, m_vertices[index].y + m_worldCenter.y);
	}

	void ConvexHull::CalculateNormals()
	{
		const unsigned int numVertices = m_vertices.size();

		if(m_normals.size() != numVertices)
			m_normals.resize(numVertices);

		for(unsigned int i = 0; i < numVertices; i++) // Dots are wrong
		{
			unsigned int index2 = i + 1;

			// Wrap
			if(index2 >= numVertices)
				index2 = 0;

			m_normals[i].x = -(m_vertices[index2].y - m_vertices[i].y);
			m_normals[i].y = m_vertices[index2].x - m_vertices[i].x;
		}
	}

	void ConvexHull::RenderHull(float depth)
	{
		if(m_renderLightOverHull)
			return;

		glBegin(GL_TRIANGLE_FAN);

		const unsigned int numVertices = m_vertices.size();

		for(unsigned int i = 0; i < numVertices; i++)
		{
			Vec2f vPos(GetWorldVertex(i));
			glVertex3f(vPos.x, vPos.y, depth);
		}

		glEnd();
	}

	void ConvexHull::CalculateAABB()
	{
		assert(m_vertices.size() > 0);

		m_aabb.m_lowerBound = m_vertices[0];
		m_aabb.m_upperBound = m_aabb.m_lowerBound;

		for(unsigned int i = 0, size = m_vertices.size(); i < size; i++)
		{
			Vec2f* pPos = &m_vertices[i];

			if(pPos->x > m_aabb.m_upperBound.x)
				m_aabb.m_upperBound.x = pPos->x;

			if(pPos->y > m_aabb.m_upperBound.y)
				m_aabb.m_upperBound.y = pPos->y;

			if(pPos->x < m_aabb.m_lowerBound.x)
				m_aabb.m_lowerBound.x = pPos->x;

			if(pPos->y < m_aabb.m_lowerBound.y)
				m_aabb.m_lowerBound.y = pPos->y;
		}

		m_aabb.CalculateHalfDims();
		m_aabb.CalculateCenter();

		m_aabbCalculated = true;
	}

	bool ConvexHull::HasCalculatedAABB() const
	{
		return m_aabbCalculated;
	}

	void ConvexHull::SetWorldCenter(const Vec2f &newCenter)
	{
		m_worldCenter = newCenter;

		m_aabb.SetCenter(m_worldCenter);

		TreeUpdate();
	}

	void ConvexHull::IncWorldCenter(const Vec2f &increment)
	{
		m_worldCenter += increment;

		m_aabb.IncCenter(increment);

		TreeUpdate();
	}

	void ConvexHull::SetRotation(float rotation)
	{
	    m_rotation = rotation;

        m_vertices.clear();

        for (unsigned int v = 0; v < m_origVertices.size(); v++)
        {
            m_vertices.push_back(m_origVertices[v]);
            m_vertices[v].RotateBy(rotation);
        }

        CalculateNormals();
    }

	Vec2f ConvexHull::GetWorldCenter() const
	{
		return m_worldCenter;
	}

	float ConvexHull::GetRotation() const
	{
	    return m_rotation;
	}

	bool ConvexHull::PointInsideHull(const Vec2f &point)
	{
		int sgn = 0;

		for(unsigned int i = 0, numVertices = m_vertices.size(); i < numVertices; i++)
		{
			int wrappedIndex = Wrap(i + 1, numVertices);
			Vec2f currentVertex(GetWorldVertex(i));
			Vec2f side(GetWorldVertex(wrappedIndex) - currentVertex);
			Vec2f toPoint(point - currentVertex);

			float cpd = side.Cross(toPoint);

			int cpdi = static_cast<int>(cpd / fabs(cpd));

			if(sgn == 0)
				sgn = cpdi;
			else if(cpdi != sgn)
				return false;
		}

		return true;
	}

	void ConvexHull::DebugDraw()
	{
		const unsigned int numVertices = m_vertices.size();

		glTranslatef(m_worldCenter.x, m_worldCenter.y, 0.0f);

		glBegin(GL_LINE_LOOP);

		for(unsigned int i = 0; i < numVertices; i++)
			glVertex2f(m_vertices[i].x, m_vertices[i].y);
	}
}
