#pragma once

//!  The rig that maintains the cameras in a single entity
/*!
  * \author R. Bertozzi & S. Bougeois
  * \brief The rig that maintains the cameras in a single entity
  * \file Rig.hpp
*/

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.hpp"

#include <iostream>
#include <cmath>

/*!
 * \brief The rig that maintains the cameras in a single entity
 */ 
class Rig
{
	public:
		//! Constructor
		/*!
		 * \param camera_one The first camera
		 * \param camera_two The second camera
		 * \param position The position of the rig
		 * \param up The up vector of the rig
		 * \param target The target vector of the rig
		 * \param width The width of the window
		 * \param height The height of the window
		 */ 
                Rig(Camera* camera_one, Camera* camera_two, glm::vec3 position, float dioc, float dc, glm::vec3 up, glm::vec3 target, int width, int height);
		//! Destructor
		~Rig();
		
		//! Sets the position of the rig
		/*!
		 * \param position The position to be written
		 */ 
		void set_position(const glm::vec3 position);
		//! Sets the up vector of the rig
		/*!
		 * \param up The up vector to be written
		 */
		void set_up(const glm::vec3 up);
		//! Sets the target vector of the rig
		/*!
		 * \param up The target vector to be written
		 */
		void set_target(const glm::vec3 target);
		
		//! Updates the position of the rig thanks to the user's controls
		/*!
		 * \param direction The direction entered by the user (0,1,2,3)
		 */
		void update_position(const int direction, const float delta);
		//! Updates the horizontal angle of the rig thanks to the user's mouse
		/*!
		 * \param mouse_x The x-coordinate of the cursor in the window
		 */
		void update_horizontal_angle(const int mouse_x);
		//! Updates the vertical angle of the rig thanks to the user's mouse
		/*!
		 * \param mouse_y The y-coordinate of the cursor in the window
		 */
		void update_vertical_angle(const int mouse_y);
		//! Updates the target of the rig
		void update_target();
		 
		//! Gets the position of the rig
		/*!
		 * \return The position of the rig
		 */ 
		glm::vec3 get_position() const;
		//! Gets the target of the rig
		/*!
		 * \return The target of the rig
		 */ 
		glm::vec3 get_target() const;
		
	private:
		Camera* m_camera_one;
		Camera* m_camera_two;
		
		glm::vec3 m_position;
		glm::vec3 m_up;
		glm::vec3 m_target;
		glm::vec3 m_right;
		
		float m_speed;
		float m_mouse_speed;
		float m_horizontal_angle;
		float m_vertical_angle;
		
		int m_width;
		int m_height;
};
