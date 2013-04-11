#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

/* This class contains all of the update and render
 * logic for the application. Very basic.
 */
class Application
{

public:
	/* Initialize the application. Create necessary
	 * buffers and load textures, etc.
	 */
	void Initialize();

	/* Perform an update. This is where the state
	 * of the application happens. Check what
	 * keys are pressed, etc.
	 */
	void Update(const double delta);

	/* Render the scene.
	 */
	void Render();
};

#endif
