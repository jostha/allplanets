#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>

const int SCREEN_WIDTH  = 1024;
const int SCREEN_HEIGHT = 768;

typedef struct {
	double x;
	double y;
} point;

typedef struct {
	char name[8];
	float orbitalPeriod;		// days
	float stepSize;			// self-calculates into radians for sin and cos
	float currentAngle;		// should be double for cos and sin functions
	float distFromSun;		// AU - converts to km before animation starts
	float dia;					// km << all distances should use same measurement!
	point centre;
} planet;

int main() {

	ALLEGRO_DISPLAY *display = NULL;		// Setup graphics window
	ALLEGRO_EVENT_QUEUE *evqueue;			// Setup events queue
	ALLEGRO_TIMER *timer;

	// Initialise Allegro
   al_init();
   display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
   al_init_primitives_addon();
	evqueue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60.0);

	al_install_keyboard();
	al_register_event_source(evqueue, al_get_keyboard_event_source());
	al_register_event_source(evqueue, al_get_display_event_source(display));
	al_register_event_source(evqueue, al_get_timer_event_source(timer));

	// Some vars
	int i, j;					// general for loops and such
	float zoom = 0.000001;

	// Planet table init
	planet planets[9] = {{"Mercury",    87.970, 0.0, 0.0, 0.387,   4879, (5,10)},
			  					{"Venus"  ,   224.700, 0.0, 0.0, 0.722,  12104, (20,10)},
			  					{"Earth"  ,   365.256, 0.0, 0.0, 1.000,  12742, (10, 15)},
								{"Mars"   ,   686.980, 0.0, 0.0, 1.520,   6779, (20,20)},
			  					{"Jupiter",  4332.590, 0.0, 0.0, 5.200, 139820, (20,10)},
			  					{"Saturn" , 10759.220, 0.0, 0.0, 9.580, 116460, (10,10)},
								{"Uranus" , 30685.400, 0.0, 0.0,19.200,  50724, (20,20)},
								{"Neptune", 60189.000, 0.0, 0.0,30.100,  49224, (10, 15)},
								{"Pluto"  , 90465.000, 0.0, 0.0,39.500,   2376, (20,20)}};

	/* populate planets with degree steps required per earth day */
	for (i=0; i<9; i++){
		planets[i].stepSize = 360.0/planets[i].orbitalPeriod;				// calc degrees
		planets[i].stepSize = planets[i].stepSize * 3.14 / 180;			// calc radians
		planets[i].distFromSun = 149597870.7 * planets[i].distFromSun;
	}
	//864,938 miles (1.392 million km).

	ALLEGRO_EVENT event;

	// Main loop
	bool running = true;
	while (running){
		al_get_next_event(evqueue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			running = false;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if(event.keyboard.keycode == ALLEGRO_KEY_UP)
            {
					zoom += 0.0000001;
	         }
			else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
            {
					zoom -= 0.0000001;
	         }
		}

		// Draw sun out of loop (I draw this twice to save extra calc, first as a 1 pixel version)
		al_draw_filled_circle( SCREEN_WIDTH/2 , SCREEN_HEIGHT/2, 2, al_map_rgb(255, 255, 0));
		al_draw_filled_circle( SCREEN_WIDTH/2 , SCREEN_HEIGHT/2, 1392700 * zoom, al_map_rgb(255, 255, 0));
		for(j=0; j<9; j++){
			planets[j].currentAngle += planets[j].stepSize;
				if (planets[j].currentAngle > 360) {
				planets[j].currentAngle -= 360;
			}
			// x = r cos(t)   radius r , angle t
			planets[j].centre.x = (zoom * planets[j].distFromSun) * cos(planets[j].currentAngle);	
			// y = r cos(t)	radius r , angle t
			planets[j].centre.y = (zoom * planets[j].distFromSun) * sin(planets[j].currentAngle);	
			al_draw_filled_circle(planets[j].centre.x, planets[j].centre.y, planets[j].dia * zoom, al_map_rgb(0, 255, 0));
			if (planets[j].dia*zoom < 1) {
				al_draw_filled_circle(planets[j].centre.x + SCREEN_WIDTH/2, planets[j].centre.y + SCREEN_HEIGHT/2, 2, al_map_rgb(0, 255, 0));
			}
		}
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
	al_destroy_display(display);

	return 0;
}
