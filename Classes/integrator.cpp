/**
 * @file    integrator.cpp
 * @author  James Sturgis
 * @date    April 8, 2018
 *
 * Implementation of an integrator object that can be created, the parameters
 * used for the integration manipulated or extracted and steps of integration
 * run. Currently it runs an NVT integration on the configuration.
 */

#include <math.h>
#include "integrator.h"
#include "common.h"
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * Constructor function that takes as a parameter the force field that will be
 * used to calculate the energy of the system during integration.
 *
 * @param forces The force field to use for energy calculations
 */
integrator::integrator(force_field *forces) {
    n_good     =
    n_bad      =
    rot_flag   = false; 
    n_step     = 0;
    n_try	=200; //this parameter was created to change the move behaviour after n_try.
    dl_max     = 1.0; 
    initial_dl_max = 1.0; //this parameters allow us to reassign obj_dl_max of object after n_steps
    i_adjust   = 1000;
    the_forces = forces;
}

/**
 * Constructor function that will copy an integrator and make a new one. That
 * is identical to the original.
 *
 * @param orig The integrator to copy.
 */
integrator::integrator(const integrator& orig) {
    n_good     = orig.n_good;
    n_bad      = orig.n_bad;
    rot_flag   = orig.rot_flag;
    dl_max     = orig.dl_max;
    initial_dl_max = orig.initial_dl_max;
    n_step     = orig.n_step;
    n_try      = orig.n_try;
    i_adjust   = orig.i_adjust;
    the_forces = orig.the_forces;
}

/**
 * Destructor to destroy an integrator.
 */
integrator::~integrator() {
}

/**
 * @brief Function to run a series of steps of integration on a configuration.
 *
 * Currently this integration is performed by at each time point:
 * - If necessary adjusting the integrator parameters and resetting the tallies.
 * - Moving an object in the configuration.
 * - Working out which parts of the energy need to be re-evaluated.
 * - Calculating a new energy for the configuration.
 * - Accepting or rejecting the configuration based on the metropolis criterion.
 * - Updating the integrator tallies.
 *
 * @param state_h a handle to the configuration. This will be updated during
 *                the run, so the_state at the end is different if a change is
 *                made.
 * \param beta    The reciprocal temperature (scaled by the boltzman constant)
 *                to use for the integration.
 * \param P       The pressure.
 * @param n_steps The number of requested steps to make.
 * @param r_list  The radius of object (fixed by NVT command)
 * @return        The total number of steps so far performed.
 *
 */
int
integrator::run(config **state_h, double beta, double P, int n_steps, float r_list){
    int     i;              ///< Iteration counter
    int     obj_number;     ///< Index of object to modify
    double  dU;             ///< Internal energy change.
    double  prob_new;       ///< Acceptance probability.
    config  *the_state = *state_h;
    config  *new_state;     ///< Pointer to modified state.
    bool    debug = false;   ///< Debuging option to control acceptance criteria.

    
    for(i = 0; i < n_steps; i++){
        /* If necessary adjust integrator parameters and tallies */
        if((n_step > 0) && ((n_step % i_adjust)== 0)){
            if(((float)n_good/(n_good+n_bad)) < 0.1) dl_max /= 3.9;
            if(((float)n_good/(n_good+n_bad)) > 0.7) dl_max *= 3.0;
            dl_max = simple_min( dl_max, the_state->x_size);
            dl_max = simple_min( dl_max, the_state->y_size);
            
            //don't let the dl_max down too much
            dl_max = simple_max(dl_max, 0.1);
            n_good = n_bad = 0;
    }
    	//reset the dl_max of each object every n steps.
    	if((n_step > 0) && ((n_step % 2000)== 0)){
            //set obj_dl_max to each object
            for (int obj_number = 0; obj_number < the_state->n_objects(); obj_number++){
                the_state->set_obj_dl_max(obj_number, initial_dl_max);
            }
        }
        /** Clone configuration and move an object in the new configuration */
        /** @todo   Chose between different types of modification           */
        new_state = new config(*the_state);
        
        /// The integrator move function.
        obj_number = rnd_lin(1.0)*the_state->n_objects();
        
        // follow primary_move() if the selected object doesn't try a specific number of move 
        if((the_state->objects_ngood(obj_number)+the_state->objects_nbad(obj_number))<n_try){
            new_state->primary_move(obj_number, dl_max, rot_flag);
            //printf("Old Algo \n");
        // follow second model of move when the selected object try to move n_try times.
        } else{
            new_state->move_aftern_primary_move(obj_number, rot_flag);
            //printf("New Algo \n");
         }
         
        new_state->invalidate_within(the_forces->cut_off, obj_number);
        new_state->unchanged = false;
        
        /* Calculate probability of accepting the new state                */
        dU = new_state->energy(the_forces, r_list)
                - the_state->energy(the_forces, r_list);
        prob_new = exp(- beta * dU );
        prob_new = simple_min(1.0,prob_new);
        
    	if(debug) {
		FILE *fptr;   
    	        fptr = fopen("NEW_algorithme_movement.txt", "a");/*  open for writing */ 
	}
        /* Accept or reject the new state according to the probability     */
        if(rnd_lin(1.0)<= prob_new ){
            if(debug) {
	        fprintf(fptr," n_step = %d accepted", n_step);
	    }
            n_good++;
            delete(the_state);               
            the_state = new_state;
            
            //increase obj_n_good of the selected object & increase the obj_dl_max
            the_state->modif_mobility(obj_number, true, initial_dl_max);
        } else {
	    if(debug) {
                fprintf(fptr," n_step = %d refused", n_step);
	    }
            n_bad++;
            delete(new_state); 
             
            //increase obj_n_bad of the selected object & reduce the obj_dl_max 
            the_state->modif_mobility(obj_number, false, initial_dl_max);
        }
	if(debug) {  
            fclose(fptr);
        }
        n_step++;
    }
    *state_h = the_state;
    return n_step;
}
