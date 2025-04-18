/**
 * @file        config.cpp  Implementation of the configuration class.
 * @brief       Implementation of the configuration class to handle molecular organizations.
 * @author      James Sturgis
 * @date        April 6, 2018
 * @version     1.0
 *
 * 30 december 2019 - implement non-rectangular areas (necessarily aperiodic)
 * Signaled in file format by x_size == y_size == 0.0.
 * Configuration boundary is then given by a polygon in the following lines
 *	N_vertice: number of vertices in polygon (second line of file)
 *	x y: coordinates of vertices one per line.
 */

#include <float.h>
#include <math.h>
#include <iostream>
#include "config.h"
#include <boost/format.hpp>
#include <string>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using boost::format;

/**
 * Constructor that produces an empty basic configuration. This is not
 * currently much use as there are not all the necessary functions for
 * manipulating the configuration.
 */
config::config() {
    x_size       = 1.0;
    y_size       = 1.0;
    unchanged    = true;
    saved_energy = 0.0;
    obj_list.resize(0);
    the_topology = (topology *)NULL;
    is_periodic  = false;
    is_rectangle = true;
    n_vertex     = 0;
    poly         = (polygon *)NULL;
}

/**
 * Constructor that reads the configuration from a file. The format of this
 * file is described in the class description.
 *
 * @param src   An file descriptor open for reading that contains the
 *              configuration to be read.
 *
 * @todo        Define file format for configuration. Currently:
 *              x_size, y_size \n
 *              n_objects \n
 *              o_type x_pos y_pos rotation \n one line per object.
 *              Would like to:
 *              - Include comments
 *              - Be space tolerant
 *              - Have a default rotation for backward compatibility.
 * @todo        Handle errors in the input file or file reading in a sensible
 *              way. If there is an error this should be apparent even if the
 *              structure is still valid.
 * @todo        Read from file if periodic conditions or not.
 * @todo        Integrate an object constructor from a file saves 4 variables
 *              and a couple of lines of code.
 */
config::config(std::istream& source ){
    config_read(source);
}

config::config(string in_file) {
    // Open a stream of the file
    ifstream ff(in_file.c_str());
    config_read( ff );
    ff.close();
}

/**
 * This is messy as defined in topology.cpp
 */
bool my_getline(std::istream& ff, string *line);

/*
 * Read a configuration file as a stream. 
 * Want to enhance this to:
 * 1. Deal sensibly with errors in the input file
 *    In case of errors it throws runtime_error() with a useful message!
 * 2. Ignore empty lines, and comments "# to end of line"
 *    These are removed by my_getline(ff, line)
 */
void
config::config_read(std::istream& ff){
    int         n_obj;
    int         o_type;
    double      x_pos, y_pos, angle;
    object      *my_obj;
    // Plus a string for each line
    string      line;
    
    is_periodic  = true;                             // Set up defaults.
    is_rectangle = true;
    n_vertex     = 0;
    poly         = (polygon *)NULL;

    // Check if the file exists
    if(ff.fail()) {
        throw runtime_error("Could not open configuration file\n");
    }
    
    // We get each line with my_getline(in_file, line)
    // This fills string with the content of the next line that is not all
    // whitespace or comment and returns true if it succeeded and false on failure.

    if(!my_getline(ff, &line)){			// Found end of file before content.
        throw runtime_error("Found no content in the configuration file\n");
    }
    istringstream iss(line);
    if (!(iss >> x_size >> y_size)) {
        throw runtime_error("First line of the configuration file should be x_size y_size, exiting ...\n");
    }
    iss.clear();

    // Here we handle the case of a non-rectangular configuration signalled
    // by 2 zeros in the first line (after any comments etc)... 
    if( x_size == 0.0 ){
        is_rectangle = false;
        is_periodic = false;

        if(! my_getline(ff, &line))
            throw runtime_error("Failed to read number of vertices..\n");
        iss.str(line);
        if( !(iss >> n_vertex )){
            throw runtime_error("Failed to read number of vertices...\n");
        }
        iss.clear();
        poly = new polygon( n_vertex );

        double x_coord, y_coord;
        for( int i = 0; i < n_vertex; i++){
            if(!my_getline(ff, &line))
        	throw runtime_error("Error reading bounding polygon coordinates..\n");
            iss.str(line);
            if( !(iss >> x_coord >> y_coord )){
        	throw runtime_error("Error reading bounding polygon coordinates...\n");
            }
            poly->add_vertex( x_coord, y_coord );
            iss.clear();
        }
    }
 
    // Next line the number of objects in the configuration
    if( !my_getline(ff, &line))
        throw range_error("Failed to read number of objects..\n");
    iss.str(line);
    if (!(iss >> n_obj))
        throw range_error("Failed to read number of objects...\n");
    iss.clear();
    
    // Now loop through the objects and remaining lines
    for( int i = 0; i < n_obj; i++){
        if( !my_getline(ff, &line))
            throw runtime_error("Problem in the coordinates..\n");
        iss.str(line);
        if (!(iss >> o_type >> x_pos >> y_pos >> angle))
            throw runtime_error("Problem in the coordinates...\n");
        
        my_obj = new object(o_type, x_pos, y_pos, angle );
        obj_list.push_back(*my_obj);
        delete my_obj;
        iss.clear();
    }
    unchanged = false;                          // Set up so will calculate energy.
    saved_energy = 0.0;
    the_topology = (topology *)NULL;            // Topologies are not included
    assert(n_obj == n_objects() );              // Include some extra tests.
}

/**
 * Copy constructor
 * @param orig the original configuration to be copied.
 */
config::config(config& orig) {
    x_size         = orig.x_size;
    y_size         = orig.y_size;
    saved_energy   = orig.saved_energy;
    unchanged      = orig.unchanged;
    if( orig.the_topology ) 
        the_topology   = new topology(orig.the_topology);
    else
        the_topology = NULL;
    is_periodic    = orig.is_periodic;
    obj_list.resize(orig.obj_list.size());
    for(int i = 0; i < (int)obj_list.size(); i++){
        obj_list[i].assign(orig.obj_list[i]);
    }
    // Add non-periodic bits
    is_rectangle   = orig.is_rectangle;
    n_vertex       = orig.n_vertex;
    if( orig.poly )
        poly       = new polygon( orig.poly );
    else
        poly       = NULL;
}

/**
 * Copy constructor
 * @param orig pointer to the original configuration to be copied.
 */
config::config(config* orig) {
    x_size         = orig->x_size;
    y_size         = orig->y_size;
    saved_energy   = orig->saved_energy;
    unchanged      = orig->unchanged;
    if( orig->the_topology ) 
        the_topology   = new topology(orig->the_topology);
    else
        the_topology = NULL;
    is_periodic    = orig->is_periodic;
    obj_list.resize(orig->obj_list.size());
    for(int i = 0; i < (int)obj_list.size(); i++){
        obj_list[i].assign(orig->obj_list[i]);
    }
    // Add non-periodic bits
    is_rectangle   = orig->is_rectangle;
    n_vertex       = orig->n_vertex;
    if( orig->poly )
        poly       = new polygon( orig->poly );
    else
        poly       = NULL;
}

/**
 * Destructor. Destroy the configuration releasing memory. As constructor
 * uses new probably need explicit destroy.
 */
config::~config() {
    if(the_topology) delete(the_topology);
    if(poly) delete(poly);
}

/**
 * @return The area of the configuration.
 *
 */
double config::area() {
    if( is_rectangle )
        return x_size * y_size;
    else
        return poly->area();
}

/**
 * This function allow us to dodge all the calcul for the energy of all interaction 
 * if the distance between two objects it's higher than the length.
 * @return booleen value after calculating if we needed to calculate all the interaction.
 *
 */
bool config::need_calculate_energy(double obj1_pos_x, double obj1_pos_y, double obj2_pos_x, double obj2_pos_y, double length, float r_list){
    //read the radius from the NVT command to fix a radius for the object
    float dist_bt_pts = 0.0;
    float dist = 0.0;
    
    // distance between points
    dist_bt_pts = sqrt((obj2_pos_x - obj1_pos_x) * (obj2_pos_x - obj1_pos_x) + (obj2_pos_y - obj1_pos_y) * (obj2_pos_y - obj1_pos_y));
    
    //distance minus radius of each object
    dist =  dist_bt_pts - r_list*2;
    
    if (dist > length){
        return false;
    }else{
        return true;
        }
    
}


/**
 * This function calculates the energy of a configuration by comparing using
 * the force field interaction function to measure the energy between pairs of
 * objects. As both indexes run from 0 to the end all interactions are counted
 * twice. This is done so that neighbour lists, if implemented, will work more
 * easilly. To increase the energy the object recalculate flag, and the
 * configuration unchanged flag are checked to reduce unnecessary evaluations
 * as long as these flags are correctly and efficiently updated.
 * This function call the function need_calculate_energy to check if we need to
 * do the calcul of each atoms interaction of an moving object.
 *
 * @param  the_force the force field to use for the energy calculation.
 * @return the total interaction energy between all object pairs.
 * @todo   Handle periodic conditions.
 */
double config::energy(force_field *&the_force, float r_list) {
    int     i1, i2;                         // Two counters
    double  value = 0.0;                    // An accumulator that starts at 0.0
    object  *my_obj1, *my_obj2;             // Two object pointers

    if (! unchanged) {                      // Only if necessary
        saved_energy = 0.0;                 // Loop over the objects
                                            // This code needs optimizing.
        for(i1 = 0; i1 < (int)obj_list.size(); i1++ ){
            my_obj1 = &obj_list[i1];
            if( my_obj1->recalculate ){
                value = 0.0;
                for(i2 = 0; i2<(int)obj_list.size(); i2++ ){
                    if(i1 != i2){            // If periodic then
                        double r, r2;
                        double dx = 0.0;
                        double dy = 0.0;

                        my_obj2 = &obj_list[i2];
                        if(is_periodic){     // Move my_obj2 to closest image
                                             // Check this code...
                            r  = my_obj2->pos_x - my_obj1->pos_x;
                            dx = (r<0)?x_size:-x_size;
                            r2 = r + dx;
                            dx = (abs(r2)<abs(r))?dx:0.0;
                            my_obj2->pos_x += dx;

                            r  = my_obj2->pos_y - my_obj1->pos_y;
                            dy = (r<0)?y_size:-y_size;
                            r2 = r + dy;
                            dy = (abs(r2)<abs(r))?dy:0.0;
                            my_obj2->pos_y += dy;
                        }
                        
                        //if radius is define and is superior to 0 in NVT
                        if (r_list > 0){
                      
                            //check if we need to calcul each interaction between the 2 object.
                            bool decision_maker = need_calculate_energy(my_obj1->pos_x, my_obj1->pos_y, my_obj2->pos_x, my_obj2->pos_y, the_force->length, r_list);
                            if (decision_maker){
                            
                            	 //calculate energy for all interaction
                                value += my_obj1->interaction( the_force, the_topology, my_obj2 );
                            }
                         //radius not define
                        }else{
                            value += my_obj1->interaction( the_force,the_topology, my_obj2);
                        }
                        

                        if(is_periodic){    // And move back again.
                            my_obj2->pos_x -= dx;
                            my_obj2->pos_y -= dy;
                        }
                    }
                }                           // Calculate interaction with wall
                if(! is_periodic ){         // if not periodic conditions.
                    if( is_rectangle )
                        value += my_obj1->box_energy( the_force, the_topology,
                            x_size, y_size );
                    else
                        value += my_obj1->box_energy( the_force, the_topology,
                            poly );
                }
                my_obj1->set_energy(value); // Set the energy of the object
            }                               // End of the recalculation.
            value = my_obj1->get_energy();  // Get object energy
            saved_energy += value;          // Add into the sum
        }                                   // End of loop over objects
        unchanged = true;                   // Value is correct mark as unchanged.
    }

    return saved_energy/2.0;                // All interactions are counted twice.
}

/**
 * Write the current configuration to a file in a format that can be used to
 * reinitialize a configuration with the file based constructor. See the class
 * description for the file format.
 *
 * @param dest  This is a file descriptor that should be open for writing.
 * @return      Should return exit status (currently always OK).
 *
 * @todo        Incorporate error handling and exit status return that is
 *              correct.
 * @todo        More c++ style!
 */

int config::write( FILE *dest ){
    if( is_rectangle )
        fprintf( dest, "%9f %9f \n", x_size, y_size );
    else {
        fprintf( dest, "%9f %9f \n", 0.0, 0.0 );
        poly->write( dest );
    }
    fprintf( dest, "%d\n", (int)obj_list.size());
    for(int i = 0; i< (int)obj_list.size(); i++){    // For each object in configuration
        obj_list[i].write(dest);              // Write it to the file -- Pass the stream
    }
    return EXIT_SUCCESS;                    // Return all well
}

/**
 * Write the current configuration to a file in a format that can be used to
 * reinitialize a configuration with the file based constructor. See the class
 * description for the file format.
 *
 * @param dest  This is an output file stream.
 * @return      Should return exit status (currently always OK).
 *
 * @todo        Incorporate error handling and exit status return that is
 *              correct.
 */

int config::write(std::ostream& dest){
    // Put the header and number of objects inside
    // Using boost for formatting, which seems the proper way in c++
    if( is_rectangle )
        dest << format("%9f %9f \n") % x_size % y_size;
    else {
        dest << format("%9f %9f \n") % 0.0 % 0.0;
        poly->write( dest );
    }
    dest << format("%d\n") % obj_list.size();

    for(int i = 0; i< (int) obj_list.size(); i++){    // For each object in configuration
        obj_list[i].write(dest);              // Write it to the file -- Pass the stream
    }
    return EXIT_SUCCESS;                    // Return all well
}

/**
 * @return The number of objects found in the configuration.
 */
int config::n_objects(){
    return obj_list.size();                 // Get size of object list.
}

/**
 * @return The n_good number of object found in the configuration. Important to print out 
 * the number of accepted move done by a specific object.
 */
int config::objects_ngood(int obj_number){
    return obj_list[obj_number].obj_n_good;
}

/**
 * @return The n_bad number of object found in the configuration. Important to print out 
 * the number of refused move of a specific object.
 */
int config::objects_nbad(int obj_number){
    return obj_list[obj_number].obj_n_bad;                 // Get n_bad of object.
}

/**
 * @return The number of rotation done by an object.
 */
int config::objects_nrot(int obj_number){
    return obj_list[obj_number].obj_n_rotation;                 // Get n_rotation of object.
}

/**
 * @return The number of translation done by an object.
 */
int config::objects_ntranslation(int obj_number){
    return obj_list[obj_number].obj_n_translation;                 // Get n_translation of object.
}

/**
 * set dl_max for a specific object in the list.
 */
void config::set_obj_dl_max(int obj_number, int dl_max){
    obj_list[obj_number].obj_dl_max = dl_max;                 // update the dl_max of the object.
}


/**
 * This function tests if there is a clash between the 2 objects obj1 and obj2
 *
 * @param obj1 a pointer to the first object.
 * @param obj2 a pointer to the second object.
 * @return true if there is a clash otherwise false.
 */
bool
config::test_clash( object *obj1, object *obj2 ){

    double  theta1 = obj1->orientation;
    double  theta2 = obj2->orientation;
    int     o_type1 = obj1->o_type;
    int     o_type2 = obj2->o_type;

    double  t1, t2, dx1, dx2, dy1, dy2, r1, r2, x1, x2, y1, y2;
    double  dx, dy, r;

    if( ! the_topology ){		// No topology (so no size) just points.
        return(( obj1->pos_x == obj2->pos_x ) && ( obj1->pos_y == obj2->pos_y ));
    }

    for(int j = 0; j < the_topology->molecules(o_type2).n_atoms; j++ ){
                                        // Get atom information
        t2  =  the_topology->molecules(o_type2).the_atoms(j).type;
        dx2 =  the_topology->molecules(o_type2).the_atoms(j).x_pos;
        dy2 =  the_topology->molecules(o_type2).the_atoms(j).y_pos;
        r2  =  the_topology->atom_sizes(t2);      // Get radius
                                        // Calculate atom position
        x2  =  obj2->pos_x + dx2 * cos(theta2) - dy2 * sin(theta2);
        y2  =  obj2->pos_y + dx2 * sin(theta2) + dy2 * cos(theta2);

        for( int k = 0; k < the_topology->molecules(o_type1).n_atoms; k++ ){
                                        // Get atom information
            t1  =  the_topology->molecules(o_type1).the_atoms(k).type;
            dx1 =  the_topology->molecules(o_type1).the_atoms(k).x_pos;
            dy1 =  the_topology->molecules(o_type1).the_atoms(k).y_pos;
            r1  =  the_topology->atom_sizes(t1);  // Get radius
                                        // Calculate atom position
            x1  =  obj1->pos_x + dx1 * cos(theta1) - dy1 * sin(theta1);
            y1  =  obj1->pos_y + dx1 * sin(theta1) + dy1 * cos(theta1);

            dx = (x2-x1);
            dy = (y2-y1);
                                        // Handle periodic conditions
            if( is_periodic ){          // Find closest image
                if( dx > (x_size - r1 - r2 )) dx -= x_size;
                if( dx < (r1 + r2 - x_size )) dx += x_size;
                if( dy > (y_size - r1 - r2 )) dy -= y_size;
                if( dy < (r1 + r2 - y_size )) dy += y_size;
            }
            r = dx*dx+dy*dy;
            if( r < ((r1+r2)*(r1+r2))) return true;
        }
    }
    return false;
}

/**
 * This function tests if there are any clashes the configuration using the
 * topology file but not the forcefield
 *
 * @return true if there is a clash otherwise false.
 */
bool
config::test_clash(){
    object *obj1;
    object *obj2;

    for(int i=0;i<(int)obj_list.size();i++){
        obj1 = &obj_list[i];
        for(int j=0; j<i; j++){
            obj2 = &obj_list[j];
            if( test_clash( obj1, obj2 )) return true;
        }
    }
    return false;
}

/**
 * This function tests if the new_object can be inserted into the configuration
 * without generating a clash (as determined by the topology file).
 *
 * @param new_object a pointer to a valid object to test for insertion.
 * @return true if there is a clash otherwise false.
 */
bool
config::test_clash( object *new_object ){
    int max_o_type = the_topology->n_atom_types - 1 ;
    int o_type1 = simple_min( new_object->o_type, max_o_type );
    double theta1 = new_object->orientation;
    double t1, dx1, dy1, r1, x1, y1;
    object *obj1;

    if( !is_periodic ){                     // Check clash with walls.
        for(int i = 0; i < the_topology->molecules(o_type1).n_atoms; i++ ){
            t1  =  the_topology->molecules(o_type1).the_atoms(i).type;
            dx1 =  the_topology->molecules(o_type1).the_atoms(i).x_pos;
            dy1 =  the_topology->molecules(o_type1).the_atoms(i).y_pos;
            r1  =  the_topology->atom_sizes(t1);  // Get radius
                                            // Calculate atom position
            x1  =  new_object->pos_x + dx1 * cos(theta1) - dy1 * sin(theta1);
            y1  =  new_object->pos_y + dx1 * sin(theta1) + dy1 * cos(theta1);
            if( is_rectangle ){
                if(( x1 < r1 ) || ( (x1 + r1) > x_size ) || ( y1 < r1 ) ||
                    ( (y1 + r1) > y_size ))
                    return true;
            } else {
                if( ! poly->is_inside( x1, y1, r1 )) return true;
            }
        }
    }
                                            // Loop over the objects.
    for(int i = 0; i < n_objects(); i++){
        obj1 = &obj_list[i];
        if(test_clash( obj1 ,new_object)) return true;
    }
    return false;
}

/**
 * Count the number of different types of object are found in the current
 * configuration. Actually it just returns the highest object type number found.
 *
 * @todo    Clean up semantics. What is actually needed and return this.
 * @return  as an integer the total number of different object types found.
 *
 */
int config::object_types(){
    int     max_type = -1;

    for(int i = 0; i< (int) obj_list.size(); i++ ){
        max_type = simple_max(max_type, obj_list[i].o_type);
    }
    assert(max_type>=0);
    return max_type;
}

/**
 * Private member function to verify that the config structure is internally
 * valid.
 *
 * Internal validations (should) include:
 * - the object_list is a valid list.
 * - all objects in the list are valid.
 * - all objects are within the area of the configuration.
 * - if 'unchanged' is true then saved_energy is the energy obtained by
 *   calculation. This is hard to check as usually the force field is not
 *   available to the check function.
 *
 * On entry to and on exit from all of the functions in the config class
 *  this->check() should return 'true'.
 *
 * @return true or false depending on evaluation.
 *
 * @todo  Implement this function.
 */
bool config::check(){
    return true;
}

/**
 * This function compares atom by atom the current configuration and the
 * reference configuration and calculates the root mean square distance between
 * the atoms.
 *
 * @param ref   a reference configuration
 * @return      as a double the rms distance
 *
 * @todo        Currently just a stub, returns 1.0
 */
double config::rms(const config& ref){
    return 0.0;
}

/**
 * This function changes the size of a configuration by an isometric expansion
 * moving all the objects apart. It does not change the orientations of the
 * various objects.
 *
 * @param dl    The multiplicative factor to apply to the size and the object
 *              coordinates.
 * @return      Return if there are clashes.
 */
bool config::expand(double dl){
    int     i;

    if( is_rectangle ){
        x_size *= dl;                           // Expand boundary
        y_size *= dl;
    } else {
        poly->expand( dl );
    }
    unchanged = false;                      	// The energies will be different
    for(i=0;i<(int)obj_list.size();i++){
        obj_list[i].recalculate = true;		// Also for the objects
        obj_list[i].expand(dl);        		// Move objects in rescaled box
    }
    return (test_clash());
}

/**
 * This function changes the size of a configuration by an isometric expansion
 * moving all the objects apart. It does not change the orientations of the
 * various objects.
 *
 * @param dl    The multiplicative factor to apply to the size and the object
 *              coordinates.
 * @param max_try Number of attempted object displacements to remove clashes.
 * @return      Return if there are clashes.
 * @todo        Jiggle and jolt (move and twist to remove clashes) steepest descent
 */
bool config::expand(double dl, int max_try ){
    int     i;

    if( is_rectangle ){
        x_size *= dl;                           // Expand boundary
        y_size *= dl;
    } else {
        poly->expand( dl );
    }
    unchanged = false;                      // The energies will be different
    for(i=0;i<(int)obj_list.size();i++){
        obj_list[i].recalculate = true;// Also for the objects
        obj_list[i].expand(dl);        // Move objects in rescaled box
    }
    for(i=0;i<max_try;i++){
      if(test_clash()) jiggle();
    }
    return test_clash();
}

/**
 * Generate an angle values for a rotation according to the object symetry. This function is usefull for the 
 * uniform_distribution of "rnd_rotate()" to generate symetrically centered distributions.
 * TODO : a swich value "-q" is a boolean to decide wether or not the rotation motion should be apply
 * It will be better to give a predifined value corespondig to the object symetries: eg 0 for a circular object
 * 3 for a trimer, 4 for a tetramer and so on.
 */
 
int   
config::side_object(int number_of_side){
    // This function prefigure a fouction to derrive symetries from an external information eg. topology 
    // The number of side indeed correspond to this symetry value (4 for tetramers)
    // It is used to restrict angle arround gaussian distributions whose stdev depend of the obj_mobility factor.
    // The more the rotation success rate decrease the more the rotation destribition restrict arround 360%number_of side

    return number_of_side;
}

/**
 * generate an angle of rotation for an object according to it's symetry.
 * @return angle to rotate an object (radian).
 */
 
float   
config::rnd_rotate(float obj_mobility){
    // Create a random device and use it to generate a random seed.
    std::random_device myRandomDevice;
    unsigned seed = myRandomDevice();
    int symet = 4; // for a tetramer. TO DO : implement that in the -q switch in place of the boolean
    int angmod = 360 / symet ;
    
    // Initialize a default_random_engine with the seed.
    std::default_random_engine generator(seed);
    
    /* append an random angle to rotate  */
    /* Initialize a uniform_int_distribution to produce values between 0 and the side object number */
    std::uniform_int_distribution<int> uniform_distri(0, side_object(symet));
    float angle = uniform_distri(generator);
    angle *= angmod;
    std::normal_distribution<double> distribution_nor(angle, angmod*obj_mobility);
    angle = distribution_nor(generator);
    angle *= M_PI/180;
    /* don't let the angle to rotate down under 1° */
    if (angle < 0.01745){
        angle = 0.01745;
    }
    return angle;
}

/**
 * Generate a decision maker to do a choice between translation, rotation or both. More the obj_n_good attribut of a selected object 
 * is high and more chances it's had to do translation. More the obj_n_bad attribut of a selected object is high and more chances 
 * it's had to do a rotation. This function use a "random" generator, so during all steps of the simulation : the objects have a chance
 * to do all the possible movement, even if they favorised one. 
 *
 * @param obj_number the index of the object to move.
 * @param decision_making_factor number to fix a limit between the event (translation, rotate, both) (actually 1 means we do both, 0 means once or the other).
 *
 * @return decision_maker it's a number between [-1;1] , it allow to determine the choice (-1 : we only do rotation, 1: we only do translation, 0 : we do both
 */
 
int   
config::trans_over_rot(int obj_number, int decision_making_factor){
    int     decision_maker;
    int     result; 
    decision_maker = 0;
    
    // Create a random device and use it to generate a random seed
    std::random_device myRandomDevice;
    unsigned seed = myRandomDevice();
    
    // Initialize a default_random_engine with the seed.
    std::default_random_engine generator(seed);
    
    /* Use uniform_distribution to determine the next movement : translation, rotate, both */
    /* We use a uniform distribution between obj_n_good & obj_n_bad to generate a decision maker */
    std::uniform_int_distribution<int> uniform_distri_mobility(-(obj_list[obj_number].obj_n_bad), (obj_list[obj_number].obj_n_good));
    result = uniform_distri_mobility(generator);
    
    // if decision_maker >= -obj_n_bad, we can propose a translation.
    if (result >= obj_list[obj_number].obj_n_bad*(-decision_making_factor)){
    	decision_maker += 1;
    }
    // if decision_maker <= obj_n_good, we can propose a rotation.	
    if (result <= obj_list[obj_number].obj_n_good*(decision_making_factor)){
    	decision_maker -=1 ;
    }
    return decision_maker;
}
   
/**
 * Move a given object to a new place using the scaling factor dl_max
 * to control the distance distribution.
 *
 * @param obj_number the index of the object to move
 * @param dl_max the scaling parameter.
 */
void config::primary_move(int obj_number, double dl_max, bool rot_flag){
    double  dist, angle;
    double  dx, dy;
    FILE *fptr;   
    fptr = fopen("NEW_algorithme_movement.txt", "a");/*  open for writing */

    /* Calculate shift distance */
    dist = rnd_lin(1.0);
    if (dist == 0.0) dist=DBL_MIN;
    dist = -2.0*log(dist);
    dist *= dl_max;
    
    /* Use angle to calculate coordinate shift */
    /* translation */
    angle = rnd_lin(1.0);
    dy = dist * cos(M_2PI*angle);
    dx = dist * sin(M_2PI*angle);
    obj_list[obj_number].move(dx, dy);
    obj_list[obj_number].obj_n_translation += 1; 
    
    if (rot_flag) {
        /* rotation */
        angle = rnd_lin(2*M_2PI)-M_2PI;
        obj_list[obj_number].rotate(angle);
        obj_list[obj_number].obj_n_rotation += 1;
    }
    fprintf(fptr,"\nobject number :  %d ,obj_n_bad : %d, obj_n_good : %d, obj_dl_max ;= %f, dx: %f, dy: %f, angle: %f", obj_number ,obj_list[obj_number].obj_n_bad, obj_list[obj_number].obj_n_good, obj_list[obj_number].obj_dl_max, dx, dy, angle);
    
    fclose(fptr);
    // Fix boundary conditions periodic or not.
    fix_inbox( obj_number );
    
}
    
/**
 * Move a given object to a new place using the scaling factor dl_max
 * to control the distance distribution. The movement are favorised
 * by a high ratio "obj_n_good/obj_all_movement" of an object and the rotation is favorised when the 
 * ratio reduce. Random rotation is added to the object according to 
 * it's shape. The amplitude of rotation varies depending on the mobility 
 * factor of each object. The less cluttered the object is, the greater the
 * rotational movements can be. same things for the translation.
 *
 *
 * @param obj_number the index of the object to move
 */
void config::move_aftern_primary_move(int obj_number, bool rot_flag){
    float  dist, angle;
    double  dx = 0, dy = 0;
    float  obj_mobility, obj_all_movement;
    float  decision_making_factor = 0.5;
    int     decision_maker;
    FILE *fptr;   
    fptr = fopen("NEW_algorithme_movement.txt", "a");/*  open for writing */ 
    
    /* Calculate shift distance */
    dist = rnd_lin(1.0);
    if (dist == 0.0) dist=DBL_MIN;
    dist = -2.0*log(dist);
    dist *= obj_list[obj_number].obj_dl_max;

	
    
    /* Use uniform_distribution to determine the next movement : if we do translation, rotate or both */
    /* if decision_maker = 0, we do the both. 1, we only do translation. -1, we just propose rotation to the object. */
    /*A decision_making_factor about 0.5 equally ballance the possibility of rotation over translation */
    
    if (rot_flag) {
        decision_maker = trans_over_rot(obj_number, decision_making_factor);
    }
    else {
        decision_maker = 1;
    }    
    fprintf(fptr,"\nobject number :  %d ,obj_n_bad : %d, obj_n_good : %d, obj_dl_max ;= %f, ", obj_number ,obj_list[obj_number].obj_n_bad, obj_list[obj_number].obj_n_good, obj_list[obj_number].obj_dl_max);
    

    /* If the decision maker is superior or egal to 0, try to propose a translation to the object */
    if (decision_maker >= 0){
    
    	/* Use angle to calculate coordinate shift */
    	angle = rnd_lin(1);
    	dy = dist * cos(M_2PI*angle);
    	dx = dist * sin(M_2PI*angle);
    	
    	/* move the object */
    	obj_list[obj_number].move(dx, dy);
    	obj_list[obj_number].obj_n_translation += 1;
    }
    fprintf(fptr,"dx: %f, dy: %f,", dx, dy);
    angle = 0;
    /* If the decision maker is inferior or egal to 0, try to rotate */
    if (decision_maker <= 0){
    
 	/* Use obj_list to calculate mobility */
	obj_all_movement = obj_list[obj_number].obj_n_good + obj_list[obj_number].obj_n_bad;
	obj_mobility = obj_list[obj_number].obj_n_good/obj_all_movement;
    
   	/* Use normal_distribution to generate the angle to rotation with quarter turns */
   	angle = rnd_rotate(obj_mobility);
    	
    	/* append the rotation angle to the object. */
    	obj_list[obj_number].rotate(angle);
    	obj_list[obj_number].obj_n_rotation += 1;
    }
    fprintf(fptr," angle: %f", angle);
    fclose(fptr);
    // Fix boundary conditions periodic or not.
    fix_inbox( obj_number );
}

/**
 * Modify the mobility ratio of the selected object by increasing obj_n_good or obj_n_bad and modified obj_dl_max.
 * When the MC algorythm accept to move an object, the obj_n_good and the obj_dl_max increase.
 * When the MC algorythm refuse a movement or a rotation of an object, the obj_n_bad of this object increase and the obj_dl_max reduce.
 *
 * @param obj_number the index of the object
 * @param bad_or_good the index to increase the obj_n_good or obj_n_bad of the selected object.
 */
void
config::modif_mobility( int obj_number, bool bad_or_good, int initial_dl_max){
    if (bad_or_good){
    	obj_list[obj_number].obj_n_good += 1;	
    	obj_list[obj_number].obj_dl_max *= 1.1;
    	
    	//don't let the obj_dl_max go higher than 
    	//obj_dl_max haven't to go upper than the initial_dl_max
    	if (obj_list[obj_number].obj_dl_max > initial_dl_max){
    	   obj_list[obj_number].obj_dl_max = initial_dl_max;	
        }
    }
    else {
    	obj_list[obj_number].obj_n_bad += 1;
    	obj_list[obj_number].obj_dl_max /= 1.1;
    	
    	// don't let the obj_dl_max go under 0.1, because we don't want a to propose a translation with : dx, dy = 0
    	//TODO obj_dl_max have to don't go under size obj *0.2 
    	if (obj_list[obj_number].obj_dl_max < 0.1){
    	   obj_list[obj_number].obj_dl_max = 0.1;
    	}
    }

}


/**
 * Translate the configuration coordinate system by dx, dy
 *
 * @param dx horizontal translation distance
 * @param dy vertical translation distance
 */
void
config::translate( double dx, double dy ){
    if( is_rectangle )
        rect_2_poly();
    poly->translate(dx,dy);
    for(int i=0; i < (int)obj_list.size(); i++ ){
    	obj_list[i].pos_x += dx;
    	obj_list[i].pos_y += dy;
    }
}

/**
 * Convert the bounding box into a polygon.
 *
 * @return true if it was possible, false otherwise.
 */
bool
config::rect_2_poly(){
    if( is_rectangle ){
        // Convert rectangle to poly.
        poly = new polygon( 4 );
        poly->add_vertex(   0.00,   0.00 );
        poly->add_vertex( x_size,   0.00 );
        poly->add_vertex( x_size, y_size );
        poly->add_vertex(   0.00, y_size );
        is_rectangle = false;
        return true;
    }
    return false;
}

/**
 * Convert a rectangular polygon bounding box to a rectangle.
 *
 * @return true if all went well, false if not either because not rectangular or nop.
 */
bool
config::poly_2_rect(){
    if( is_rectangle ) return false;

    if( !poly->is_parallelogram() ) return false;
    poly->order_vertices();
    translate(-(poly->get_vertex(0).x), -(poly->get_vertex(0).y));
    if( poly->get_vertex(1).y != 0.00 ){
        rotate( - atan2( poly->get_vertex(1).y, poly->get_vertex(1).x) );
    }
    // Now should be oriented (? good to test).
    is_rectangle = true;
    x_size = poly->get_vertex(1).x;
    y_size = poly->get_vertex(3).y;
    delete( poly );
    return true;
}

/**
 * Determine if is p is left of line s->e as the sign of the z component 
 * of the 3D cross product of s->p and s->e.
 */
bool
on_left( double px, double py, double sx, double sy, double ex, double ey ){ // Is p on the left of the line s->e
	return ((ex*py-ex*sy-sx*py-ey*px+ey*sx+sy*px)>0.0);
}

/**
 * Check if all atoms of all objects are inside the boundary
 * TODO improve this to work with periodic conditions and rectangles properly
 */
bool
config::objects_inside( polygon *a_poly ){
	object	*my_obj;
	int		o_type, t;
	double	theta, x, y, dx, dy, r;
	
	for( int i = 0; i < (int) obj_list.size(); i++ ){
        my_obj = &obj_list[i];
        theta  = my_obj->orientation;
        o_type = my_obj->o_type;
        if( o_type > object_types() ){          // Use object type 0 if not defined
            o_type = 0;
        }
        if( is_rectangle ){
        	if( my_obj->pos_x < 0.0 ) return false;
        	if( my_obj->pos_y < 0.0 ) return false;
        	if( my_obj->pos_x > x_size ) return false;
        	if( my_obj->pos_y > y_size ) return false;
        } else {
        	for(int j = 0; j < the_topology->molecules(o_type).n_atoms; j++ ){
                                            // Get atom information
            	t  =  the_topology->molecules(o_type).the_atoms(j).type;
            	dx =  the_topology->molecules(o_type).the_atoms(j).x_pos;
            	dy =  the_topology->molecules(o_type).the_atoms(j).y_pos;
            	r  =  the_topology->atom_sizes(t);      // Get radius
            	                                // Calculate atom position
            	x  =  my_obj->pos_x + dx * cos(theta) - dy * sin(theta);
            	y  =  my_obj->pos_y + dx * sin(theta) + dy * cos(theta);
            	if( !a_poly->is_inside(x, y, r))
                    return false;
            }
        }
	}
	return true;
}

/**
 * @brief Calculate convex hull around objects
 * Finds a collection of points, that determine a convex perimeter enclosing
 * all the objects in the configuration.
 *
 * @param expand should the hull be expanded to enclose finite sized objects.
 * @return The convex hull calculated as a polygon.
 */
polygon		*
config::convex_hull(bool expand){
    polygon *a_poly = new polygon();
    assert(obj_list.size()>=3);
    
    int	left_most = 0;
    double x_min = obj_list[0].pos_x;
    for( int i = 0; i < (int)obj_list.size(); i++ ){
    	if( obj_list[i].pos_x < x_min ){
    		x_min = obj_list[i].pos_x;
    		left_most = i;
    	}
    }

    int pointOnHull = left_most;
    int endpoint = 0;
    int	i = 0;

    do{
    	a_poly->add_vertex(obj_list[pointOnHull].pos_x,  obj_list[pointOnHull].pos_y);
    	endpoint = (pointOnHull == 0)?1:0;
    	for( int j = 0; j < (int)obj_list.size(); j++ ){
    		if( j != pointOnHull){
    		    if( on_left( obj_list[j].pos_x, obj_list[j].pos_y,
    			    	a_poly->get_vertex(i).x, a_poly->get_vertex(i).y,
    				    obj_list[endpoint].pos_x, obj_list[endpoint].pos_y) )
    		        endpoint = j;
    		}
    	}
    	i++;
    	pointOnHull = endpoint;
    } while (endpoint != left_most);

	if(expand){									// If necessary increase size to include objects Don't think this works
	    
	    double	dl = 0.15 * the_topology->atom_sizes(0);	// A reasonable length to grow by.
	    double	px, py;										// vector to previous vertex x,y
	    double  qx, qy;										// vector to next vertex x,y
	    double  r;											// for normalization
	    polygon	*new_poly = new polygon();
	    int		count = a_poly->n_vertex;
	    do{    	
	    	for(int i=0; i< count; i++ ){
	    		px = a_poly->get_vertex((i-1) < 0?a_poly->n_vertex-1:i-1).x -  a_poly->get_vertex(i).x;
	    		py = a_poly->get_vertex((i-1) < 0?a_poly->n_vertex-1:i-1).y -  a_poly->get_vertex(i).y;
	    		r  = sqrt( px*px+py*py );
	    		px /= r;									// Adjust to unit length
	    		py /= r;
	    		qx = a_poly->get_vertex((i+1) >= a_poly->n_vertex?0:i+1).x  -  a_poly->get_vertex(i).x;
	    		qy = a_poly->get_vertex((i+1) >= a_poly->n_vertex?0:i+1).y  -  a_poly->get_vertex(i).y;
	    		r  = sqrt( qx*qx+qy*qy );
	    		qx /= r;									// Adjust to unit length
	    		qy /= r;
	    		px += qx;
	    		py += qy;
	    		r  = - sqrt( px*px+py*py );
	    		px *= dl/r;									// Adjust to dl length
	    		py *= dl/r;
	    		new_poly->add_vertex(a_poly->get_vertex(i).x + px, a_poly->get_vertex(i).y + py);
	    	}
    		delete a_poly;
	   		a_poly = new polygon(new_poly);
	   		delete new_poly;
	   		new_poly = new polygon();
	    } while (!objects_inside( a_poly ));
		delete new_poly;
	}	

	return a_poly;
}

/**
 * @brief Set the boundary to the provided polygon
 * @param a_poly is a pointer to the new boundary object.
 */
void
config::set_poly(polygon *a_poly){
	if(is_rectangle){
		is_rectangle = false;
	} else {
		delete poly;
	}
	poly = a_poly;
	unchanged = false;
	is_periodic  = false;
}

/**
 * Ensure that the object is inside the boundary. If using periodic conditions
 * use these to fix it. If not periodic conditions move onto boundary?
 *
 * @param obj_number the index of the object to work on.
 */
void config::fix_inbox( int obj_number ){

    double pos_x = obj_list[obj_number].pos_x;
    double pos_y = obj_list[obj_number].pos_y;

    if( is_periodic ){
        while( pos_x < 0 )      pos_x += x_size;
        while( pos_x > x_size ) pos_x -= x_size;
        while( pos_y < 0 )      pos_y += y_size;
        while( pos_y > y_size ) pos_y -= y_size;
    } else {
        if( is_rectangle ){
            while( pos_x < 0 )      pos_x += x_size/2.0;
            while( pos_x > x_size ) pos_x -= x_size/2.0;
            while( pos_y < 0 )      pos_y += y_size/2.0;
            while( pos_y > y_size ) pos_y -= y_size/2.0;
	    } else {
            while(! poly->is_inside(pos_x, pos_y)){
                pos_x -= (pos_x - poly->center_x())/2.0;
                pos_y -= (pos_y - poly->center_y())/2.0;
            }
	    }
    }

    obj_list[obj_number].pos_x = pos_x;
    obj_list[obj_number].pos_y = pos_y;
}

/**
 * Rotate an object designated by the obj_number a random angle
 * scaled by theta_max.
 *
 * @param obj_number The index of the object to move
 * @param theta_max The scaling parameter.
 */
void config::rotate(int obj_number, double theta_max){
    double angle = rnd_lin(theta_max)-theta_max/2.0;
    obj_list[obj_number].rotate(angle);
}

/**
 * Rotate the configuration (bounding polygon and objects) clockwise by angle
 *
 * @param angle the number of radians to rotate
 */
void
config::rotate( double angle ){
    if( is_rectangle ) rect_2_poly();
    poly->rotate( angle );
    for(int i=0; i< n_objects(); i++){
    	  /// TODO fix positions
        /// calculate new xy coordinates TODO
        obj_list[i].rotate( -angle );
    }    
}



/**
 * Mark as needing recalculation of energies all objects within a certain
 * distance of a reference object.
 *
 * @param distance the cut-off distance to use.
 * @param index the number of the reference object.
 */
void    config::invalidate_within(double distance, int index){
    object  *obj1;
    object  *obj2;

    obj1 = &obj_list[index];
    for(int i=0; i< n_objects(); i++)       // For each object in the configuration
      if (i!= index){                       // That is difference
        obj2 = &obj_list[i];             // Check distance
        if( obj1->distance(obj2, x_size, y_size, is_periodic) < distance ) // TODO: Need to check works for non-rectangles
            obj2->recalculate = true;       // and set flag if necessary
    }
}

/** \brief Associate a topology with the configuration
 *
 * \param a_topology a pointer to the topology.
 *
 */

void    config::add_topology(topology* a_topology){
    if( the_topology )                      // If there is already one
        delete( the_topology );             // Get rid of it
    the_topology = a_topology;              // Make the new association
}

/**
 * \brief Return the width of a configuration (independant of boundary).
 *
 * \return the calculated width.
 */
double
config::width(){
   if( is_rectangle ) return x_size;
   else return (poly->x_max() - poly->x_min());
}

/**
 * \brief Return the height of a configuration (independant of boundary).
 *
 * \return the calculated height.
 */
double
config::height(){
   if( is_rectangle ) return y_size;
   else return (poly->y_max() - poly->y_min());
}

/** \brief Insert an object into the configuration.
 *
 * \param orig the object to add
 * \return nothing
 *
 */
void    config::add_object(object* orig ){
    obj_list.push_back(*orig);
}

/** \brief Fetch object from list by index
 *
 *  \param index the index of the object in the list
 *  \return the_object
 */
object	*config::get_object( int index ){
    assert( index < n_objects() );
    return &obj_list[index];
}

/** \brief Output a postscript snippet to draw the configuration
 *
 * \param the_forces forcefield, needed for atom sizes and colors.
 * \param dest the file for the output.
 * \return no return value
 *
 * \todo use return value for error handling.
 */

void    
config::ps_atoms( std::ostream& dest ){
/*  Need to reorganize for topology with molecules...
    should incorporate atom draw radius in topology (so no FF needed)... 

    TODO handle is_periodic and is_rectangle correctly
*/

    object  *my_obj;
    double  theta, dx, dy, r, x, y;
    int     t, lr, tb;
    char    *my_color;
    int     max_o_type, o_type;
    
    if ( !the_topology ){
        throw runtime_error(
        "Generating postscript images from a configuration requires setting a topology.");
    }

    max_o_type = the_topology->n_atom_types -1 ;
                                            // Loop over the objects.
    for(int i = 0; i < (int)obj_list.size(); i++){
        my_obj = &obj_list[i];
        theta  = my_obj->orientation;
        o_type = my_obj->o_type;
        if( o_type > max_o_type ){          // Use object type 0 if not defined
            o_type = 0;
        }
                                            // Loop over the atoms
        for(int j = 0; j < the_topology->molecules(o_type).n_atoms; j++ ){
                                            // Get atom information
            t  =  the_topology->molecules(o_type).the_atoms(j).type;
            dx =  the_topology->molecules(o_type).the_atoms(j).x_pos;
            dy =  the_topology->molecules(o_type).the_atoms(j).y_pos;
            r  =  the_topology->atom_sizes(t);      // Get radius
                                            // Calculate atom position
            x  =  my_obj->pos_x + dx * cos(theta) - dy * sin(theta);
            y  =  my_obj->pos_y + dx * sin(theta) + dy * cos(theta);
            my_color = the_topology->molecules(o_type).the_atoms(j).color;
                                            // Write postscript snippet for atom.
            dest << "newpath " << x << " " << y << " moveto "
                 << my_color << " " << r << " fcircle \n";

                                            // Handle intersections with the border
            if( is_periodic ){
                lr = 0; tb = 0;                 // need up to 4 copies.
                if ( x < r ) lr = -1;
                if ( x > (x_size - r)) lr = +1;
                if ( y < r ) tb = -1;
                if ( y > (y_size - r)) tb = +1;
                if (lr != 0 ){                  // Copy on other side
                    dest << format("newpath %g %g moveto %g %s fcircle \n") 
                         % (x-lr*x_size) % y % r % (my_color);
                }
                if (tb != 0 ){                  // Vertical copy
                    dest << format("newpath %g %g moveto %g %s fcircle \n") 
                        % x % (y-tb*y_size) % r % (my_color);
                }
                if ((lr != 0 )&&(tb != 0)){     // In the corner!
                    dest << format("newpath %g %g moveto %g %s fcircle \n") 
                        % (x-lr*x_size) % (y-tb*y_size)
                        % r % ( my_color );
                }
            }
        }
    }
}

bool
config::has_clash( int i ){
    for(int j=0; j< (int) obj_list.size(); j++ ){
        if (i!=j) {
            if( test_clash( &obj_list[i], &obj_list[j] )) return true;
        }
    }
    return false;
}

/***
 * @brief Shake objects a bit to try and remove bad contacts
 */
void
config::jiggle(){
    double  dist, angle;
    double  dx, dy;

    for( int i=0; i < (int)obj_list.size(); i++){
        if( has_clash( i )){
            /* Calculate shift distance */
            dist = rnd_lin(1.0);
            if (dist == 0.0) dist=DBL_MIN;
            dist = -2.0*log(dist);
            /* Use angle to calculate coordinate shift */
            angle = rnd_lin(1.0);
            dy = dist * cos(M_2PI*angle);
            dx = dist * sin(M_2PI*angle);

            obj_list[i].move(dx,dy);
            fix_inbox( i );
            angle = rnd_lin(M_2PI)-M_PI;
            obj_list[i].rotate(angle);
        }
    }
}

