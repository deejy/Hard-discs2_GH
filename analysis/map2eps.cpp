/**
 * @file    map2eps.cpp
 * @author  James Sturgis
 * @date    February 3, 2020
 * @version 1.0
 * @brief   Convert a map/matrix to an eps figure.
 *
 * This file map2eps program that is part of the Very Coarse Grained disc simulation 
 * program suite.
 *
 * The program converts a map file, read from the input, to an encapuslated
 * postscript file containing a representation of the configuration in which each cell
 * of the matrix is colored according to the number of objects found in that position.
 * Optionally the cell can also contain an arrow indicating the averaged orientation
 * of the objects at that position.
 *
 * Usage:
 *          map2eps [-c color_name] [-a] < map_file > eps_file.
 *
 */

#include <iostream>
#include <unistd.h>

using namespace std;

void
usage(){
    cerr << "Usage: map2eps [-c color_name] [-a] < map_file > eps_file\n";
}

string prolog = ""
"%!PS-Adobe-2.0 EPSF-1.2 \n"
"%%Title: Postscript figure for map \n"
"%%Creator: James \n"
"%%CreationDate: Today \n"
"%%DocumentFonts: (atend) \n";

string ps_dict = ""
"/mydict 256 dict def \n"
"mydict begin \n"
"/Color true def \n"
"/Solid true def \n"
"/Rounded false def \n"
" \n"
"/gnulinewidth 5.000 def \n"
"/userlinewidth gnulinewidth def \n"
"/vshift 0 def \n"
"/dl {10.0 mul} def \n"
"/hpt_ 1.0 def \n"
"/vpt_ 1.0 def \n"
"/hpt hpt_ def \n"
"/vpt vpt_ def \n"
"/symbolsize 0.5 def \n"
" \n"
"/vpt2 vpt 2 mul def \n"
"/hpt2 hpt 2 mul def \n"
" \n"
"/Lshow { currentpoint stroke moveto 0 vshift rmoveto show } def \n"
"/Rshow { currentpoint stroke moveto dup stringwidth pop neg vshift rmoveto show } def \n"
"/Cshow { currentpoint stroke moveto dup stringwidth pop -2 div vshift rmoveto show } def \n"
" \n"
"/UP { dup vpt_ mul /vpt exch def hpt_ mul /hpt exch def \n"
"  /hpt2 hpt 2 mul def /vpt2 vpt 2 mul def } def \n"
"/DL { Color {setrgbcolor Solid {pop []} if 0 setdash } \n"
" {pop pop pop 0 setgray Solid {pop []} if 0 setdash} ifelse } def \n"
"/BL { stroke userlinewidth 2 mul setlinewidth \n"
"      Rounded { 1 setlinejoin 1 setlinecap } if } def \n"
"/AL { stroke userlinewidth 2 div setlinewidth \n"
"      Rounded { 1 setlinejoin 1 setlinecap } if } def \n"
"/UL { dup gnulinewidth mul /userlinewidth exch def \n"
"      dup 1 lt {pop 1} if 10 mul /udl exch def } def \n"
"/PL { stroke userlinewidth setlinewidth \n"
"      Rounded { 1 setlinejoin 1 setlinecap } if } def \n"
" \n"
"% Plotting stuff \n"
"/minorticksize 0.015 def \n"
"/majorticksize minorticksize 2.0 mul def \n"
" \n"
"/xminortick { 0 moveto 0.0 minorticksize rlineto  \n"
"   0.0 1.0 2.0 minorticksize mul sub rmoveto  \n"
"   0.0 minorticksize rlineto } def \n"
"/xmajortick { 0 moveto 0.0 majorticksize rlineto  \n"
"   0.0 1.0 2.0 majorticksize mul sub rmoveto  \n"
"   0.0 majorticksize rlineto 0.0 -1.0 rmoveto } def \n"
"/yminortick { 0 exch moveto minorticksize 0.0 rlineto \n"
"   1.0 2.0 minorticksize mul sub 0.0 rmoveto \n"
"   minorticksize 0.0 rlineto -1.0 0.0 rmoveto } def \n"
"/ymajortick { 0 exch moveto majorticksize 0.0 rlineto \n"
"   1.0 2.0 majorticksize mul sub 0.0 rmoveto \n"
"   majorticksize 0.0 rlineto -1.0 0.0 rmoveto } def \n"
" \n"
"% Linetypes \n"
"/LTw { PL [] 1 setgray } def \n"
"/LTb { BL [] 0 0 0 DL } def \n"
"/LTa { AL [1 udl mul 2 udl mul] 0 setdash 0 0 0 setrgbcolor } def \n"
"/LT0 { PL [] 0.7 0 0 DL } def \n"
"/LT1 { PL [40. 20.] 0 0.6 0 DL } def \n"
"/LT2 { PL [20. 30.] 0 0 0.7 DL } def \n"
"/LT3 { PL [10. 15.] 1 0 1 DL } def \n"
"/LT4 { PL [50. 20. 10. 20.] 0 1 1 DL } def \n"
"/LT5 { PL [40. 30. 10. 30.] 1 1 0 DL } def \n"
"/LT6 { PL [20. 20. 20. 40.] 0 0 0 DL } def \n"
"/LT7 { PL [20. 20. 20. 20. 20. 40.] 1 0.3 0 DL } def \n"
"/LT8 { PL [20. 20. 20. 20. 20. 20. 20. 40.] 0.5 0.5 0.5 DL } def \n"
" \n"
"/IndianRed {0.803922 0.360784 0.360784 setrgbcolor } def \n"
"/LightCoral {0.941176 0.501961 0.501961 setrgbcolor } def \n"
"/Salmon {0.980392 0.501961 0.447059 setrgbcolor } def \n"
"/DarkSalmon {0.913725 0.588235 0.478431 setrgbcolor } def \n"
"/LightSalmon {1 0.627451 0.478431 setrgbcolor } def \n"
"/Crimson {0.862745 0.0784314 0.235294 setrgbcolor } def \n"
"/Red {1 0 0 setrgbcolor } def \n"
"/FireBrick {0.698039 0.133333 0.133333 setrgbcolor } def \n"
"/DarkRed {0.545098 0 0 setrgbcolor } def \n"
"/Pink {1 0.752941 0.796078 setrgbcolor } def \n"
"/LightPink {1 0.713725 0.756863 setrgbcolor } def \n"
"/HotPink {1 0.411765 0.705882 setrgbcolor } def \n"
"/DeepPink {1 0.0784314 0.576471 setrgbcolor } def \n"
"/MediumVioletRed {0.780392 0.0823529 0.521569 setrgbcolor } def \n"
"/PaleVioletRed {0.858824 0.439216 0.576471 setrgbcolor } def \n"
"/LightSalmon {1 0.627451 0.478431 setrgbcolor } def \n"
"/Coral {1 0.498039 0.313725 setrgbcolor } def \n"
"/Tomato {1 0.388235 0.278431 setrgbcolor } def \n"
"/OrangeRed {1 0.270588 0 setrgbcolor } def \n"
"/DarkOrange {1 0.54902 0 setrgbcolor } def \n"
"/Orange {1 0.647059 0 setrgbcolor } def \n"
"/Gold {1 0.843137 0 setrgbcolor } def \n"
"/Yellow {1 1 0 setrgbcolor } def \n"
"/LightYellow {1 1 0.878431 setrgbcolor } def \n"
"/LemonChiffon {1 0.980392 0.803922 setrgbcolor } def \n"
"/LightGoldenrodYellow {0.980392 0.980392 0.823529 setrgbcolor } def \n"
"/PapayaWhip {1 0.937255 0.835294 setrgbcolor } def \n"
"/Moccasin {1 0.894118 0.709804 setrgbcolor } def \n"
"/PeachPuff {1 0.854902 0.72549 setrgbcolor } def \n"
"/PaleGoldenrod {0.933333 0.909804 0.666667 setrgbcolor } def \n"
"/Khaki {0.941176 0.901961 0.54902 setrgbcolor } def \n"
"/DarkKhaki {0.741176 0.717647 0.419608 setrgbcolor } def \n"
"/Lavender {0.901961 0.901961 0.980392 setrgbcolor } def \n"
"/Thistle {0.847059 0.74902 0.847059 setrgbcolor } def \n"
"/Plum {0.866667 0.627451 0.866667 setrgbcolor } def \n"
"/Violet {0.933333 0.509804 0.933333 setrgbcolor } def \n"
"/Orchid {0.854902 0.439216 0.839216 setrgbcolor } def \n"
"/Fuchsia {1 0 1 setrgbcolor } def \n"
"/Magenta {1 0 1 setrgbcolor } def \n"
"/MediumOrchid {0.729412 0.333333 0.827451 setrgbcolor } def \n"
"/MediumPurple {0.576471 0.439216 0.858824 setrgbcolor } def \n"
"/RebeccaPurple {0.4 0.2 0.6 setrgbcolor } def \n"
"/BlueViolet {0.541176 0.168627 0.886275 setrgbcolor } def \n"
"/DarkViolet {0.580392 0 0.827451 setrgbcolor } def \n"
"/DarkOrchid {0.6 0.196078 0.8 setrgbcolor } def \n"
"/DarkMagenta {0.545098 0 0.545098 setrgbcolor } def \n"
"/Purple {0.501961 0 0.501961 setrgbcolor } def \n"
"/Indigo {0.294118 0 0.509804 setrgbcolor } def \n"
"/SlateBlue {0.415686 0.352941 0.803922 setrgbcolor } def \n"
"/DarkSlateBlue {0.282353 0.239216 0.545098 setrgbcolor } def \n"
"/MediumSlateBlue {0.482353 0.407843 0.933333 setrgbcolor } def \n"
"/GreenYellow {0.678431 1 0.184314 setrgbcolor } def \n"
"/Chartreuse {0.498039 1 0 setrgbcolor } def \n"
"/LawnGreen {0.486275 0.988235 0 setrgbcolor } def \n"
"/Lime {0 1 0 setrgbcolor } def \n"
"/LimeGreen {0.196078 0.803922 0.196078 setrgbcolor } def \n"
"/PaleGreen {0.596078 0.984314 0.596078 setrgbcolor } def \n"
"/LightGreen {0.564706 0.933333 0.564706 setrgbcolor } def \n"
"/MediumSpringGreen {0 0.980392 0.603922 setrgbcolor } def \n"
"/SpringGreen {0 1 0.498039 setrgbcolor } def \n"
"/MediumSeaGreen {0.235294 0.701961 0.443137 setrgbcolor } def \n"
"/SeaGreen {0.180392 0.545098 0.341176 setrgbcolor } def \n"
"/ForestGreen {0.133333 0.545098 0.133333 setrgbcolor } def \n"
"/Green {0 0.501961 0 setrgbcolor } def \n"
"/DarkGreen {0 0.392157 0 setrgbcolor } def \n"
"/YellowGreen {0.603922 0.803922 0.196078 setrgbcolor } def \n"
"/OliveDrab {0.419608 0.556863 0.137255 setrgbcolor } def \n"
"/Olive {0.501961 0.501961 0 setrgbcolor } def \n"
"/DarkOliveGreen {0.333333 0.419608 0.184314 setrgbcolor } def \n"
"/MediumAquamarine {0.4 0.803922 0.666667 setrgbcolor } def \n"
"/DarkSeaGreen {0.560784 0.737255 0.545098 setrgbcolor } def \n"
"/LightSeaGreen {0.12549 0.698039 0.666667 setrgbcolor } def \n"
"/DarkCyan {0 0.545098 0.545098 setrgbcolor } def \n"
"/Teal {0 0.501961 0.501961 setrgbcolor } def \n"
"/Aqua {0 1 1 setrgbcolor } def \n"
"/Cyan {0 1 1 setrgbcolor } def \n"
"/LightCyan {0.878431 1 1 setrgbcolor } def \n"
"/PaleTurquoise {0.686275 0.933333 0.933333 setrgbcolor } def \n"
"/Aquamarine {0.498039 1 0.831373 setrgbcolor } def \n"
"/Turquoise {0.25098 0.878431 0.815686 setrgbcolor } def \n"
"/MediumTurquoise {0.282353 0.819608 0.8 setrgbcolor } def \n"
"/DarkTurquoise {0 0.807843 0.819608 setrgbcolor } def \n"
"/CadetBlue {0.372549 0.619608 0.627451 setrgbcolor } def \n"
"/SteelBlue {0.27451 0.509804 0.705882 setrgbcolor } def \n"
"/LightSteelBlue {0.690196 0.768627 0.870588 setrgbcolor } def \n"
"/PowderBlue {0.690196 0.878431 0.901961 setrgbcolor } def \n"
"/LightBlue {0.678431 0.847059 0.901961 setrgbcolor } def \n"
"/SkyBlue {0.529412 0.807843 0.921569 setrgbcolor } def \n"
"/LightSkyBlue {0.529412 0.807843 0.980392 setrgbcolor } def \n"
"/DeepSkyBlue {0 0.74902 1 setrgbcolor } def \n"
"/DodgerBlue {0.117647 0.564706 1 setrgbcolor } def \n"
"/CornflowerBlue {0.392157 0.584314 0.929412 setrgbcolor } def \n"
"/MediumSlateBlue {0.482353 0.407843 0.933333 setrgbcolor } def \n"
"/RoyalBlue {0.254902 0.411765 0.882353 setrgbcolor } def \n"
"/Blue {0 0 1 setrgbcolor } def \n"
"/MediumBlue {0 0 0.803922 setrgbcolor } def \n"
"/DarkBlue {0 0 0.545098 setrgbcolor } def \n"
"/Navy {0 0 0.501961 setrgbcolor } def \n"
"/MidnightBlue {0.0980392 0.0980392 0.439216 setrgbcolor } def \n"
"/Cornsilk {1 0.972549 0.862745 setrgbcolor } def \n"
"/BlanchedAlmond {1 0.921569 0.803922 setrgbcolor } def \n"
"/Bisque {1 0.894118 0.768627 setrgbcolor } def \n"
"/NavajoWhite {1 0.870588 0.678431 setrgbcolor } def \n"
"/Wheat {0.960784 0.870588 0.701961 setrgbcolor } def \n"
"/BurlyWood {0.870588 0.721569 0.529412 setrgbcolor } def \n"
"/Tan {0.823529 0.705882 0.54902 setrgbcolor } def \n"
"/RosyBrown {0.737255 0.560784 0.560784 setrgbcolor } def \n"
"/SandyBrown {0.956863 0.643137 0.376471 setrgbcolor } def \n"
"/Goldenrod {0.854902 0.647059 0.12549 setrgbcolor } def \n"
"/DarkGoldenrod {0.721569 0.52549 0.0431373 setrgbcolor } def \n"
"/Peru {0.803922 0.521569 0.247059 setrgbcolor } def \n"
"/Chocolate {0.823529 0.411765 0.117647 setrgbcolor } def \n"
"/SaddleBrown {0.545098 0.270588 0.0745098 setrgbcolor } def \n"
"/Sienna {0.627451 0.321569 0.176471 setrgbcolor } def \n"
"/Brown {0.647059 0.164706 0.164706 setrgbcolor } def \n"
"/Maroon {0.501961 0 0 setrgbcolor } def \n"
"/White {1 1 1 setrgbcolor } def \n"
"/Snow {1 0.980392 0.980392 setrgbcolor } def \n"
"/HoneyDew {0.941176 1 0.941176 setrgbcolor } def \n"
"/MintCream {0.960784 1 0.980392 setrgbcolor } def \n"
"/Azure {0.941176 1 1 setrgbcolor } def \n"
"/AliceBlue {0.941176 0.972549 1 setrgbcolor } def \n"
"/GhostWhite {0.972549 0.972549 1 setrgbcolor } def \n"
"/WhiteSmoke {0.960784 0.960784 0.960784 setrgbcolor } def \n"
"/SeaShell {1 0.960784 0.933333 setrgbcolor } def \n"
"/Beige {0.960784 0.960784 0.862745 setrgbcolor } def \n"
"/OldLace {0.992157 0.960784 0.901961 setrgbcolor } def \n"
"/FloralWhite {1 0.980392 0.941176 setrgbcolor } def \n"
"/Ivory {1 1 0.941176 setrgbcolor } def \n"
"/AntiqueWhite {0.980392 0.921569 0.843137 setrgbcolor } def \n"
"/Linen {0.980392 0.941176 0.901961 setrgbcolor } def \n"
"/LavenderBlush {1 0.941176 0.960784 setrgbcolor } def \n"
"/MistyRose {1 0.894118 0.882353 setrgbcolor } def \n"
"/Gainsboro {0.862745 0.862745 0.862745 setrgbcolor } def \n"
"/LightGray {0.827451 0.827451 0.827451 setrgbcolor } def \n"
"/Silver {0.752941 0.752941 0.752941 setrgbcolor } def \n"
"/DarkGray {0.662745 0.662745 0.662745 setrgbcolor } def \n"
"/Gray {0.501961 0.501961 0.501961 setrgbcolor } def \n"
"/DimGray {0.411765 0.411765 0.411765 setrgbcolor } def \n"
"/LightSlateGray {0.466667 0.533333 0.6 setrgbcolor } def \n"
"/SlateGray {0.439216 0.501961 0.564706 setrgbcolor } def \n"
"/DarkSlateGray {0.184314 0.309804 0.309804 setrgbcolor } def \n"
"/Black {0 0 0 setrgbcolor } def \n"
" \n"
"% For color shading \n"
"/adjustChan { 1 exch sub 4 -1 roll dup 5 1 roll mul 1 exch sub } def \n"
"/adjustTint { 4 1 roll adjustChan 3 1 roll adjustChan 3 1 roll adjustChan 3 1 roll 4 -1 roll pop } def \n"
"% Symbols \n"
"/CircE { currentpoint symbolsize dup 0 rmoveto 0 360 arc stroke } def \n"
"/circle {currentpoint 3 -1 roll dup 0 rmoveto 0 360 arc stroke } def \n"
"/fcircle {currentpoint 3 -1 roll dup 0 rmoveto 0 360 arc fill } def \n"
"/BoxE  { currentpoint symbolsize dup dup rmoveto -2.0 mul dup 0  \n"
"   rlineto dup 0 exch rlineto -1 mul 0 rlineto closepath stroke } def \n"
" \n"
"/Symbol-Oblique /Symbol findfont [1 0 .167 1 0 0] makefont \n"
"dup length dict begin {1 index /FID eq {pop pop} {def} ifelse} forall \n"
"currentdict end definefont pop \n"
"end \n"
"%%EndProlog \n";

string preamble = ""
"%% \n"
"%%Page: 1 \n"
"mydict begin \n"
" \n"
"gsave          % Original \n";

string ending = ""
"grestore       % End of Figure \n"
"end \n"
"showpage \n"
"%%Trailer \n"
"%%DocumentFonts: Helvetica \n"
"%%Pages: 1 \n";

/**
 *
 */
int main(int argc, char** argv) {
    string      color_name;
    bool        verbose = false;
    bool		arrows  = false;
    bool		absolut = false;
    char        c;

	int			x_maxbin, y_maxbin;
	int			bb_x, bb_y;
	
    color_name.assign("DarkOrange");
    // Getopt based argument handling.
    while( ( c = getopt (argc, argv, "vanc:") ) != -1 )
    {
        switch(c)
        {
            case 'v': verbose = true;
                break;
            case 'a': arrows  = true;
                break;
            case 'n': absolut = true;
            	break;
            case 'c':
                if (optarg){
                    color_name.assign(optarg);
                }
                break;
            case '?':				// Something wrong.
                if (optopt == 'c' ){
                    std::cerr << "The -" << optopt << "option is missing a parameter!\n";
                } else {
                    std::cerr << "Unknown option " << optopt << "!\n";
                }
            default :                           // Something very wrong.
                usage();
                return EXIT_FAILURE;
        }
    }
    if( verbose ){                              // Report on situation
        std::cerr << "Verbose flag set\n";
        std::cerr << "Color will be " << color_name << "\n";
        std::cerr << "Arrows will" << (arrows?" ":" not ") << "be drawn.\n";
    }

    if(( argc - optind ) > 0 ){					// Check enough parameters
        std::cerr << "Error too many parameters!\n";
        usage();
        return EXIT_FAILURE;
    }
    
    // Get the map
    std::cin >> x_maxbin >> y_maxbin;
    if(verbose){
    	std::cerr << "Size is " << x_maxbin << " by " << y_maxbin <<".\n"
    		      << "Allocating arrays.\n";
    }

	// Allocate arrays
	double	**p_array;
	double	**ep_array;
	double	**n_array;
	double	**dx_array;
	double	**dy_array;

	p_array  = new double*[x_maxbin];
	ep_array = new double*[x_maxbin];
	n_array  = new double*[x_maxbin];
	dx_array = new double*[x_maxbin];
	dy_array = new double*[x_maxbin];
	for(int i=0; i< x_maxbin; i++ ){
		p_array[i]  = new double[y_maxbin];
		ep_array[i] = new double[y_maxbin];
		n_array[i]  = new double[y_maxbin];
		dx_array[i] = new double[y_maxbin];
		dy_array[i] = new double[y_maxbin];
	}
	for(int k=0; k < x_maxbin; k++)
	    for(int l=0; l< y_maxbin; l++){
	    	p_array[k][l]  =
	    	ep_array[k][l] =
	    	n_array[k][l]  =
	    	dx_array[k][l] =
	    	dy_array[k][l] = 0.0;
	}
    
    if(verbose){
    	std::cerr << "Allocated arrays... " << x_maxbin << " by " << y_maxbin << " now filling them.\n";
    }

	int		x, y;
	double	p_max = 0.0;
	double  l2_max = 0.0;
	double	len2;
	int		n_max = 0;
	int		center_x = (x_maxbin - 1)/2;
	int		center_y = (y_maxbin - 1)/2;
	
	for(int i=0; i< x_maxbin; i++){
		for(int j=0; j< y_maxbin; j++ ){
			std::cin >> x >> y >> p_array[i][j] >> ep_array[i][j] >> dx_array[i][j] >> dy_array[i][j];
			
			if((x!=i)||(y!=j)){
				std::cerr << "Fatal error reading file :" << x << " , " << y << " != " << i << " , " << j << "\n";
				return EXIT_FAILURE;
			}
			
			n_array[i][j] = p_array[i][j] * ep_array[i][j];
			if((i!=center_x) || (j!=center_y)){
				if( n_max < n_array[i][j]) n_max = n_array[i][j];
				if( p_max < p_array[i][j]) p_max = p_array[i][j];
				len2 = dx_array[i][j]*dx_array[i][j]+dy_array[i][j]*dy_array[i][j];
				if(l2_max < len2 ) l2_max = len2;
			}
		}
	}

	bb_x = 10.0 * x_maxbin;
	bb_y = 10.0 * y_maxbin;
	
    // Write the postscript
    if(verbose) std::cerr << "Writing postscript file\n";
    
    std::cout << prolog;
    std::cout << "%%BoundingBox: -5 -5 " << bb_x+5 << " " << bb_y+5 << "\n"
              << "%%EndComments \n"
              << " \n";
    std::cout << ps_dict;
    std::cout << preamble;

	for(int k=0; k < x_maxbin; k++){
	    for(int l=0; l< y_maxbin; l++){
	    	double sf = p_array[k][l]/ p_max;
	    	std::cout << "newpath " << 10 * k << " " << 10 * l << " moveto\n"
	    			<< "0 10 rlineto 10 0 rlineto 0 -10 rlineto closepath\n";
	    	if((k==center_x)&&(l==center_y)){
	    		std::cout << "Black";
	    	} else {
		    	std::cout << color_name ;
		    	std::cout << " currentrgbcolor " << sf << " adjustTint setrgbcolor";
		    }
	    	std::cout << " fill\n";
	    	std::cout << "newpath " << 10 * k << " " << 10 * l << " moveto\n"
	    			<< "0 10 rlineto 10 0 rlineto 0 -10 rlineto closepath 0 setgray 0.005 UL LTb stroke\n";
	    }
	}
    
/*
    std::cout << sf << " " << sf << " scale\n"
              << "gsave          % scaling page \n"
              << "0 setgray \n"
              << "newpath \n"
              << "0.005 UL       % set standard line width \n"
				  << "LTb            % set line color, width and dash. \n";
    current_state->poly->ps_draw(std::cout);
    std::cout << "closepath \n"
              << "stroke \n"
              << "newpath \n";
    current_state->poly->ps_draw(std::cout);
    std::cout << "closepath \n"
              << "clip \n"
              << "gsave \n";
    current_state->ps_atoms( std::cout );	// Write the different objects out
*/
    std::cout << ending;

    if(verbose) std::cerr << "Cleaning up\n";

    for(int i=0; i< x_maxbin; i++ ){
		delete[] p_array[i];
		delete[] ep_array[i];
		delete[] n_array[i];
		delete[] dx_array[i];
		delete[] dy_array[i];
	}

	delete[] p_array;
	delete[] ep_array;
	delete[] n_array;
	delete[] dx_array;
	delete[] dy_array;

    if(verbose) std::cerr << "Cleaned up\n";

    return EXIT_SUCCESS;
}
