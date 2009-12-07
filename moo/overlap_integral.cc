//
// File:   overlap_integral.cc
// Author: denis 
// calculates overlap integrals for molecular orbitals using libmoo
// usage:
//       overlap_integral --coordinates <coordinate file>
//                        --orbitals <orbitals file>
//                        --pairs <orbital pairs>
//                        --matrix <transformation matrix file>
//                        --out <output file>
//                        
// Created on 14 July 2008, 11:29
//

#include <boost/program_options.hpp>
#include <string>
#include <votca/tools/matrix.h>
#include <fock_matrix.h>
//#include <fenv.h>

int main(int argc, char **argv) {

    // molecules and orbitals 
    mol_and_orb _mol1, _mol2;

    // basis set 
    basis_set _indo;

    // define basis set 
    _mol1.define_bs(_indo);
    _mol2.define_bs(_indo);

    // molecular orbitals
    /* here we use the pointers since the _mol1 amd _mol2 call the destructors 
     * of these two objects (without creating them). This shall be changed in 
     * future by defining a method of mol_and_orb which would create an orbital
     */
    orb *_orb1, *_orb2;

    // initializing the basis set
    _orb1->set_read_orb_gauss();
    _orb2->set_read_orb_gauss();


    // catches the exceptions if the program does not behave 
    // feenableexcept(FE_DIVBYZERO | FE_OVERFLOW | FE_INVALID);

    // parser for the program arguments
    namespace po = boost::program_options;
    // declare the supported options
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("orbitals,o", po::value<string > (), "molecular orbitals file")
            ("coordinates,c", po::value<string > (), "coordinates (xyz) file")
            ("pairs,p", po::value<string > (), "file with the list of orbital pairs")
            ("matrix,m", po::value<string > (), "transformation matrix")
            ("out,j", po::value<string > (), "output file")
            ("xyz", po::value<string > (), "xyz file")
            ;

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (po::error err) {
        cout << "error parsing command line: " << err.what() << endl;
        return -1;
    }

    // otput help message and quit 
    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    // check if the file with coordinates was given and read them in 
    if (vm.count("coordinates")) {
        cout << "Reading coordinates from "
                << vm["coordinates"].as<string > () << " ... ";
        string geometry_file = vm["coordinates"].as<string > ();
        _mol1.init(geometry_file.c_str());
        _mol2.init(geometry_file.c_str());
        cout << "done\n";
    } else {
        cerr << "ERROR: File with coordinates was not specified\n";
        return -1;
    }

    // check if the file with orbitals was given and read in the orbitals
    if (vm.count("orbitals")) {
        cout << "Reading orbitals from "
                << vm["orbitals"].as<string > () << " ... ";
        string orbitals_file = vm["orbitals"].as<string > ();
        _orb1 = new orb;
        _orb2 = new orb;
        _mol1.init_orbitals(*_orb1, orbitals_file.c_str());
        _mol2.init_orbitals(*_orb2, orbitals_file.c_str());
        cout << "done\n";
        cout << "Number of electrons in the molecule: " << _mol1.n_el << endl;
    } else {
        cerr << "ERROR: File with orbitals was not specified\n";
        return -1;
    }

    vector< pair <unsigned int, unsigned int> > list_of_orbital_pairs;
    vector< unsigned int > list_of_orbitals;
    // check if the file with orbital pairs was given and read them in 
    if (vm.count("pairs")) {
        cout << "Reading orbital pairs from "
                << vm["pairs"].as<string > () << "\n";

        ifstream in_file;
        string filename = vm["pairs"].as<string > ();
        in_file.open(filename.c_str());

        pair <unsigned int, unsigned int> pair_of_orbitals;
        while (in_file >> pair_of_orbitals.first >> pair_of_orbitals.second) {
            list_of_orbital_pairs.push_back(pair_of_orbitals);
            list_of_orbitals.push_back(pair_of_orbitals.second);
        }
        in_file.close();

    } else {
        cout << "WARNING: File with the pairs was not specified, "
                "assuming HOMO-HOMO and LUMO-LUMO\n";

        for (int _orbital = _mol1.n_el / 2 - 1; _orbital <= _mol1.n_el / 2; _orbital++) {
            pair <unsigned int, unsigned int> pair_of_orbitals;
            pair_of_orbitals.first = _orbital;
            pair_of_orbitals.second = _orbital;
            list_of_orbital_pairs.push_back(pair_of_orbitals);
            list_of_orbitals.push_back(pair_of_orbitals.second);
        }
    }


    ofstream out_file;
    if (vm.count("out")) {
        cout << "Saving results to " << vm["out"].as<string > () << ".\n";
        string filename = vm["out"].as<string > ();
        out_file.open(filename.c_str());
    } else {
        cout << "WARNING: No output file generated, using stdout\n";
    }

    // initialize the rotation matrix and the translation vector
    vec separation(0.0, 0.0, 0.0);
    //double rotation[3][3];
    matrix rotation;
    
    // cout << "Initializing the fock matrix\n";
    fock _fock(_mol1, _mol2);
    _fock.set_zindo_s();

    // check if the file with transformation matrix was given and read them in 
    // calculate the transfer integral and output to either file or cout 
    if (vm.count("matrix")) {
        cout << "Reading transformation matrix from "
                << vm["matrix"].as<string > () << "\n";

        ifstream in_file;
        string filename = vm["matrix"].as<string > ();
        in_file.open(filename.c_str());

        pair <unsigned int, unsigned int> pair_of_orbitals;
        double _x, _y, _z;
        while (in_file >> _x >> _y >> _z
                >> rotation[0][0] >> rotation[0][1] >> rotation[0][2]
                >> rotation[1][0] >> rotation[1][1] >> rotation[1][2]
                >> rotation[2][0] >> rotation[2][1] >> rotation[2][2]
                ) {
            
            /*cout << rotation[0][0] << " " << rotation[0][1] << " " << rotation[0][2] << endl;
            cout << rotation[1][0] << " " << rotation[1][1] << " " << rotation[1][2] << endl;
            cout << rotation[2][0] << " " << rotation[2][1] << " " << rotation[2][2] << endl;
            */
            
            separation.setX(_x);
            separation.setY(_y);
            separation.setZ(_z);
            
            //cout << separation[0] << " " << separation[1] << " " << separation[2] << endl;
            
            // libmoo is using atomic units, deviding by the bohr radius
            separation *= 1 / RA;

            vector<double> overlap_integral;

            // rotate the orbitals of the second molecule 
            _mol2.rot_orb(rotation, list_of_orbitals);

            // rotate the atoms of the second molecule
            _mol2.rotate(rotation);
            
            // shift the second molecule
            _mol2.shift(separation);

            overlap_integral = _fock.calcJ(list_of_orbital_pairs);

            // restoring the position and the orbital of the second molecule
            _mol2.cp_atompos(_mol1);
            _mol2.cp_orb(_mol1, list_of_orbitals);

            // output in the following format: integral (orb1-orb2)
            vector < double >::iterator _integral;
            vector< pair <unsigned int, unsigned int> >::iterator _pair;
            _pair = list_of_orbital_pairs.begin();

            for (_integral = overlap_integral.begin(); _integral < overlap_integral.end(); ++_integral) {
                pair < int, int > _p = *_pair;
                if (out_file.is_open()) {
                    out_file << *_integral << " (" << _p.first << "-" << _p.second << ")" << endl;
                } else {
                    cout << *_integral << " (" << _p.first << "-" << _p.second << ")" << endl;
                }
                _pair++;
            }


        }
    } else {
        cerr << "ERROR: File with the transformation matrix was not specified\n";
        return -1;
    }

    if (out_file) {
        out_file.close();
    }

    
    return 0;

}

