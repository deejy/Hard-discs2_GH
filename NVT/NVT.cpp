/**
 * \file    NVT.cpp
 * \author  James Sturgis
 * \date    April 6, 2018
 * \version 1.0
 * \brief   Run a trajectory in the NVT ensemble.
 *
 * This file contains the main routine for the NVT program that is part of
 * the Very Coarse Grained disc simulation programmes.
 *
 * The programme loads a configuration and then runs a monte carlo integration
 * in the NVT ensemble in which for each move a random object is selected and
 * moved to a new location and rotated to a new orientation. This modified
 * configuration is accepted according to the Metropolis criterion, the
 * energy is lower or the probability of the move at the simulation temperature,
 * calculated as e^(-dE beta), is higher than a uniform variant on the interval
 * 0..1. In this equation dE is the energy difference between the old and new
 * configurations, and beta (which is a parameter to the program) is 1/ kb T
 * where kb is the boltzman constant and T the absolute temperature.
 *
 * To use the program the command line is:
 *
 *      NVT [-vpq][-t topology][-f forcefield][-o final_config][-c initial_config] [-l log_file] 
 *          [-n frame_freq] [-s traj_file] [-r r_list] n_steps print_frequency beta pressure
 *
 * Where the various parameters are:
 *      n_steps         The number of simulation steps to make.
 *      print_frequency The number of steps between reports to the log file
 *                      of how the integration is progressing.
 *      beta            The temperature parameter 1/(kb T) that scales the
 *                      force field energies.
 *      pressure        The pressure (this is not used but is for compatibility
 *                      with other ensembles such as NPT or the Gibbs ensemble).
 *
 *      -t topology     The topology file to use for the integration (required).
 *      -f forcefield   The forcefield file to use for the integration (required).
 *      -c              Optional flag for the initial configuration if none is
 *                      given the stdin will be read.
 *      initial_config  The name of an existing file containing a valid
 *                      configuration, that is read as the starting point.
 *      -o              Optional flag for the output if none is given then 
 *                      stdout will be used.
 *      final_config    The name of a file to which will be written the final
 *                      configuration, if a file with this name exists already
 *                      it is deleted.
 *      -l log_file     Optional file for logging output, if none is given then
 *                      stderr will be used.
 *
 *      -n frame_freq	The frequency to save frames to the trajectory file.
 *
 *      -s traj_file	Optional file for logging the trajectory a gzipped format.
 *
 *	-r r_list	The size of pre-filter for the distance between object. That 
 *			should be equal or less than the higher diameter of biggest object
 *
 * \todo log file       Use a dedicated function for writing data so it is easier
 *                      to parse after and control the structure.  Perhaps in
 *                      xml format.
 *
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "../Classes/integrator.h"
#include "../Classes/common.h"

#include "../Libraries/gzstream.h"

// program_options, to parse arguments
// #include <boost/program_options.hpp>
#include <boost/format.hpp>
// Smart pointer

using boost::format;

using namespace std;


#define fatal_error(format, value) {\
                    fprintf(stderr, format, value ); \
                    exit(EXIT_FAILURE); \
                }


void 
usage(int val){
    std::cerr << "NVT [-vpq][-t topology][-f forcefield][-o final_config][-c initial_config]"
        << "[-l log_file] [-n save-frequency] [-s save_file] [-r r_list] n_steps print_frequency beta pressure \n";
    exit(val);
}

/*
 *
 */
int main(int argc, char** argv) {
    
    // Use c++ string
    string       in_name;
    string       out_name;
    string       force_name;
    string       log_name;
    string       topo_name;
    string	 traj_name;

    // Objects in headers
    config      *current_state = NULL;
    config      **state_h = NULL;

    force_field *the_forces = NULL; 
    integrator  *the_integrator = NULL;
    topology    *a_topology = NULL;

    int         N1;
    double      U1, V1;
    int         i, step;
    int         c;
    bool	verbose  = false;
    bool	periodic = false;
    bool	rot_flag = false;    

    int         it_max = 0;
    int         n_print = 0;
    int		traj_freq = 0;		// Frequency for saving frames to trajectory (0=never)
    double      beta = 1.0;
    double      dl_max = 1.0;
    double      pressure = 1.0;
    float	 r_list= 0.00; //r_list is a pre-filter for the distance between object. That should be equal or less than the higher diameter of biggest object. r_list default is 0.00.  

    // Initialization

    srand((long)&argv[0]);

    // Handle command line
    while( ( c = getopt (argc, argv, "vpc:f:t:o:l:n:s:r:") ) != -1 )
    {
        switch(c)
        {
            case 'v': verbose  = true; break;
            case 'p': periodic = true; break;
            case 'q': rot_flag = true; break;
            case 'c': if (optarg) in_name = optarg;
                break;
            case 'l': if (optarg) log_name = optarg;
                break;
            case 'f': if (optarg) force_name = optarg;
                break;
            case 't': if (optarg) topo_name = optarg;
                break;
            case 'o': if (optarg) out_name = optarg;
                break;
            case 'n': if (optarg) traj_freq = std::atoi(optarg);
                break;
            case 's': if (optarg) traj_name = optarg;
                break;
            case 'r': if (optarg){
                          r_list = std::atof(optarg);
                      }
            		
                break;
            case 'h': usage(EXIT_SUCCESS);
            case '?':				// Something wrong.
                if (optopt == 'c' or optopt == 'f' or optopt == 't' or 
                    optopt == 'o' or optopt == 'l' or optopt == 'n' or
                    optopt == 's' or optopt == 'r' ){
                    std::cerr << "The -" << optopt << " option is missing a parameter!\n";
                } else {
                    std::cerr << "Unknown option " << optopt << "!\n";
                }
            default :                           // Something very wrong.
                usage(EXIT_FAILURE);
        }
    }

    std::ofstream log_file;
    #define logger ((log_file.is_open())? log_file : std::cout )
    if( log_name.length() > 0 ){
       log_file.open( log_name, std::ofstream::out );
    }

    if( verbose ) logger << "Verbose flag set\n";
    if(( log_name.length() > 0 ) && verbose ) logger << "opened " << log_name << "as logfile.";

    if(( argc - optind ) != 4 ){	        // Check enough parameters
        std::cerr << "Not right number of parameters!\n";
        usage(EXIT_FAILURE);
    }

    it_max   = std::atoi( argv[ optind++ ] );	// Find size for configuration
    n_print  = std::atoi( argv[ optind++ ] );
    beta     = std::atof( argv[ optind++ ] );
    pressure = std::atof( argv[ optind++ ] );
    
    // prompt error message if radius is under 0.00
    if(r_list < 0 ){
        std::cerr << "Negative , r_list distance invalid.\n";
	usage(EXIT_FAILURE);
    }
    
    if(it_max <= 0 ){
        std::cerr << "Nothing to do, number of steps invalid.\n";
	usage(EXIT_FAILURE);
    }
    if(n_print <= 0 ){
        std::cerr << "Negative or zero print frequency invalid.\n";
	usage(EXIT_FAILURE);
    }
    if( beta < 0 ){
        std::cerr << "Negative temperature invalid.\n";
	usage(EXIT_FAILURE);
    }
    if( pressure < 0 ){
        std::cerr << "Negative pressure invalid.\n";
	usage(EXIT_FAILURE);
    }

    if( verbose ) logger << "Reading configuration.\n";
    try{
        if( in_name.length() > 0 ){
            current_state = new config(in_name);
        } else {
            current_state = new config(std::cin);
        }
    }
    catch(...){
        std::cerr << "Error reading configuration aborting.\n";
        if( current_state ) delete current_state;
        exit( EXIT_FAILURE );
    }
    if( verbose ) logger << "Read configuration successfully.\n";

    // Load the force field from the force field file
    if( force_name.length() == 0 ){
        std::cerr << "Error the force field file was required but was not declared. Aborting.\n";
        if( current_state ) delete current_state;
        exit( EXIT_FAILURE );
    }

    if( verbose ) logger << "Reading force field from " << force_name << ".\n";
    try{
        the_forces = new force_field(force_name.c_str());
    }
    catch(...){
        std::cerr << "Error reading force field. Aborting.\n";
        delete current_state;
        if( the_forces ) delete the_forces;
        exit( EXIT_FAILURE );
    }
    if( verbose ){
        logger << "Read force_field successfully.\n";
        logger << "==============================\n";
        the_forces->write(logger);
        logger << "==============================\n";
    }
    // Load the topology from the topology file
    if( topo_name.length() == 0 ){
        std::cerr << "Error the topology file is required but was not declared. Aborting.\n";
        delete current_state;
        delete the_forces;
        exit( EXIT_FAILURE );
    }

    if( verbose ) logger << "Reading topology from" << topo_name << ".\n";
    try{
        a_topology = new topology(topo_name.c_str());
    }
    catch(...){
        logger << "Error reading topology. Aborting.\n";
        delete current_state;
        delete the_forces;
        if( a_topology ) delete a_topology;
        exit( EXIT_FAILURE );
    }
    if( verbose ){
        logger << "Read topology file successfully.\n";
        logger << "==============================\n";
        a_topology->write(logger);
        logger << "==============================\n";
    }

    // Setup to save trajectory
    ogzstream	traj_stream;

    if( traj_freq > 0 ){
        if( traj_name.length() == 0 ){
            std::cerr << "You must specify a file name for saving a trajectory (-s option)\n";
            delete current_state;
            delete the_forces;
            delete a_topology;
            exit( EXIT_FAILURE );
        }
        logger << "Snap shots saved every " << traj_freq << " steps\n";
        traj_stream.open( traj_name.c_str() );
        if( ! traj_stream.good() ){
            std::cerr << "Error while opening file " << traj_name << " for the trajectory.\n";
            delete current_state;
            delete the_forces;
            delete a_topology;
            exit( EXIT_FAILURE );
        }
        if( verbose ){
            logger << traj_name << " opened for the trajectory.\n";
        }
    } else {
        traj_freq = it_max + 1;					// Don't want a trajectory
    }

    // Add the topology to the configuration.
    current_state->add_topology(a_topology);

    if( current_state->is_rectangle ){
        current_state->is_periodic = periodic;
    } else if( periodic ){					/// TODO convert parallelogram to rectangle
        if( current_state->poly->is_parallelogram() ){
            if( current_state->poly_2_rect() ){
                current_state->is_periodic = periodic;
            }
        }
        if( ! current_state->is_rectangle ){
            std::cerr << "Periodic conditions for non-rectangular configurations not supported - ignoring flag\n";
        }
    }
    
    U1 = current_state->energy(the_forces, r_list);
    V1 = current_state->area();
    N1 = current_state->n_objects();

    // Print report of state, both in terminal and log
    logger << "After" << std::to_string( 0 ) << " steps...\n";
    logger << format("N objects = %9d Pressure = %9g   Beta = %9g\n") % N1 % pressure % beta;
    logger << format("Area      = %9g  Density = %9g Energy = %9g\n\n") % V1 % (N1/V1) % U1;

    //Calculate the global dl_max
    dl_max = simple_min(current_state->width(), current_state->height())/2.0;
    
    //Set dl_max of each object as global dl_max
    for (int obj_number = 0; obj_number < N1; obj_number++){
        current_state->set_obj_dl_max(obj_number, dl_max);
    }

    // Jiggle everything to remove bad contacts from save/load
    i = 0;          // Counter for number of shifts.
    if((U1 > the_forces->big_energy) && verbose ){
        logger << "Jiggle is necessary.\n";
    } else {
        logger << "No jiggle is necessary.\n";
    }
    
    i = 0;

    while(U1 > the_forces->big_energy){
        if( the_integrator ) delete the_integrator;
        if( i > 2000*N1 ){
            delete the_forces;
            delete current_state;
            fatal_error("Unable to adjust initial configuration in %d steps", i );
        }
        the_integrator = new integrator(the_forces);
        the_integrator->dl_max = dl_max;
        the_integrator->rot_flag = rot_flag;  // Adding -q option for rotation move
        state_h = &current_state;
        the_integrator->run(state_h, beta, pressure, 2*N1, r_list);
        current_state = *state_h;
        dl_max = the_integrator->dl_max;
        i += 2*N1;

        U1 = current_state->energy(the_forces, r_list);
        if( verbose ){
            logger << "after" << std::to_string( i ) << " steps\n";
            logger << format("N objects = %9d Pressure = %9g   Beta = %9g\n") % N1 % pressure % beta;
            logger << format("Area      = %9g  Density = %9g Energy = %9g\n\n") % V1 % (N1/V1) % U1;
        }
    }

    if( the_integrator ){
        delete the_integrator;
        i = 0;
        logger << "After initial adjustments:\n";
        logger << format("N objects = %9d Pressure = %9g   Beta = %9g\n") % N1 % pressure % beta;
        logger << format("Area      = %9g  Density = %9g Energy = %9g\n\n") % V1 % (N1/V1) % U1;
    }

    // Start NVT montecarlo loop
    // Calculate next step size...
    step = simple_min(n_print,it_max);
    step = simple_min(step, traj_freq);
    the_integrator = new integrator(the_forces);
    the_integrator->dl_max = dl_max;
    the_integrator->initial_dl_max = dl_max;

    if( verbose ){
        logger << "With" << (current_state->is_periodic?" ":"out ") << "periodic boundary conditions.\n";
        logger << "Boundary is " << (current_state->is_rectangle ? "rectangle" : "polygon") << "\n";
        logger << "Starting iteration loop\n";
    }

    for(i=0;i<it_max;){

        state_h = &current_state;
        the_integrator->run(state_h, beta, pressure, step, r_list);
        current_state = *state_h;

        U1 = current_state->energy(the_forces, r_list);
        V1 = current_state->area();
        N1 = current_state->n_objects();

        i += step;

        if( i%n_print == 0 ){				// Is it time to print to the log file
            logger << format("After %d steps N = %d, P = %g, beta = %g\n") 
                % i % N1 % pressure % beta;
            logger << format("Area = %g, Density = %g Energy = %g\n") 
                % V1 % (N1/V1) % U1;
            logger << format("Moves %d in %d, Dist_max = %g\n\n") 
                % (the_integrator->n_good)
                % (the_integrator->n_good + the_integrator->n_bad)
                % (the_integrator->dl_max);
            // just to see the parameters of each object (not usefull expect during analysis)
            for (int obj_number = 0; obj_number < N1; obj_number++){
                logger << format("obj_number = %d, n_good = %d, n_bad = %d, n rotation = %d, n translation = %d\n") 
                    % (obj_number)
                    % (current_state->objects_ngood(obj_number))
                    % (current_state->objects_nbad(obj_number))
                    % (current_state->objects_nrot(obj_number))
                    % (current_state->objects_ntranslation(obj_number));
            }
        }
        if( i%traj_freq == 0 ){				// Is it time to print to the trajectory
            traj_stream << "====" << i << "====\n";
            current_state->write( traj_stream );
        }
        
        step = simple_min(it_max-i+1,(n_print - (i%n_print)));
        step = simple_min(step,(traj_freq - (i%traj_freq)));
    }
    delete the_integrator;
    
    if( traj_stream.good() ){				// If we are writing a trajectory
        traj_stream.close();				// Close the file
    }
 
    if( verbose ) logger << "Writing final configuration.\n";
    if( out_name.length() > 0 ){
        std::ofstream out_file(out_name);
        current_state->write(out_file);
        out_file.close();
    } else {
        current_state->write(std::cout);
    }
    if( verbose ) logger << "Wrote configuration successfully.\n";

    delete current_state;
    delete the_forces;

    logger << "\n...Done...\n";

    // And close the log and output
    if( log_name.length() > 0 ){log_file.close();}

    return 0;
}
