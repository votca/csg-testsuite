// File:   test.cc
// Author: vehoff
//
// Created on June 18, 2008, 10:31 AM
//

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <votca/tools/vec.h>
#include <votca/tools/tokenizer.h>
#include <votca/tools/matrix.h>
#include <votca/tools/random.h>

#include <kmc/graph.h>
#include <kmc/hoppers.h>
#include <kmc/kmc.h>
#include <kmc/global.h>

using namespace std;

vec CalcDist(const vec& r_i, const vec& r_j, const matrix& box);
void CheckInput(double _cutoff, const matrix& box);

int main(int argc, char** argv) {
    
    clog << "Welcome to the official KMC test suite ;-)" << endl;
    clog << "Note that for standard KMC PBCs in z-direction are automatically removed." << endl;
    
    // whether to do continuous or standard KMC
    bool do_cont = true;
    
    // constants taken from moo/global.h
    static double RA=0.529189379; // Bohr radius
    const double Ang = 1E-10; // m to Ang
    const double Hartree =27.21; // Hartree to eV

    /// initialize KMC parameters
    const double J = 0.608338; // transfer integral [eV]
    const double temp = 300; // temperature [K]
    double kT = 0.025; //k_eVK * temp; // thermal energy [eV]
    const double reorg = 0.25; // reorganization energy [eV]
    vec field;
    field = vec(0.0, 0.0, 10000000.0); // electric field [V/m]
    const double dist = 0.35; // distance between molecules [nm] 
    double dG = 0; // change in energy between sites [eV]
    double w_f, w_n, w_b; // the charge transfer rates forward, neutral and back [1/sec]
    
    //initialize the random number generator
    clog << "Initializing random number generator. ";
    Random::init( 1, 2, 3, 4 ); 
    clog << "Done." << endl;
    double sigma = 0.0; // width of transfer integral distribution [eV]
    double Js = 0;
    clog << "Width of transfer integral distribution sigma = " << sigma << " [eV]" << endl;
    
    /// initialize parameters for cubic box
    const int WIDTH = 10; // box in x dir
    const int DEPTH = 10; // box in y dir
    const int HEIGHT = 10; // box in z dir
    vec a, b, c; // unit cell vectors in Bohr
    double d_bohr = dist * 10/RA;
    a = vec(d_bohr, 0.0, 0.0);
    b = vec(0.0, d_bohr, 0.0);
    c = vec(0.0, 0.0, d_bohr);
    double r_c = abs(a)*1.05; // cut-off radius
    const int Npart = WIDTH*DEPTH*HEIGHT;
    vec* particles; // the positions of the particles
    particles = new vec[Npart];
    matrix pbc;
    pbc.set(0,0,WIDTH*a.getX()); pbc.set(1,0,WIDTH*a.getY()); pbc.set(2,0,WIDTH*a.getZ());
    pbc.set(0,1,DEPTH*b.getX()); pbc.set(1,1,DEPTH*b.getY()); pbc.set(2,1,DEPTH*b.getZ());
    pbc.set(0,2,HEIGHT*c.getX()); pbc.set(1,2,HEIGHT*c.getY()); pbc.set(2,2,HEIGHT*c.getZ());
    clog << "The simulation box is given by: " << endl << pbc;
    
    /// compute the rates
    cout << "k_B T = " << kT << " [eV]" << endl;
    dG = -c.getZ()*field.getZ()*RA*Ang; // dG = E*d; negative for electrons
    cout << "dG = " << dG << " [eV]" << endl;
    w_f = sqrt(pi/(reorg*kT))*J*J*exp(-(dG+reorg)*(dG+reorg)/(4*kT*reorg))/hbar_eVs; // SI rate
    clog << "HBAR = " << hbar_eVs << " [eV]" << endl;
    clog << "Rate in direction of field = " << w_f << " [1/sec]" << endl;
    
    /// analytically approximate the mobility
    double w_eff, d_mol;
    w_b = sqrt(pi/(reorg*kT))*J*J*exp(-(-dG+reorg)*(-dG+reorg)/(4*kT*reorg))/hbar_eVs;
    w_n = sqrt(pi/(reorg*kT))*J*J*exp(-(reorg)*(reorg)/(4*kT*reorg))/hbar_eVs;
    w_eff = abs(w_f-w_b);
    clog << "Rate against the field = " << w_b << " [1/sec]" << endl;
    clog << "Rate perpendicular to the field = " << w_n << " [1/sec]" << endl;
    clog << "Effective transfer rate: w_eff = " << w_eff << " [1/sec]" << endl;
    d_mol = dist * 1E-9; // distance between molecules in meters
    clog << "Distance between molecules: d_mol = " << d_mol << " [m]" << endl;
    clog << "Electric field: E = " << field.getZ() << " [V/m]" << endl;
    clog << "Mobility: mu = " << 10000*w_eff*d_mol/field.getZ() << " [cm^2/V sec]" << endl;
    
    /// build cubic box of evenly spaced particles
    clog << "Creating cubic box of evenly spaced particles. ";
    for (int n = 0; n<WIDTH; n++){
        for (int m = 0; m<DEPTH; m++){
            for(int i = 0; i<HEIGHT; i++){
                vec pos;
                pos.setX(n*a.getX()+m*b.getX()+i*c.getX());
                pos.setY(n*a.getY()+m*b.getY()+i*c.getY());
                pos.setZ(n*a.getZ()+m*b.getZ()+i*c.getZ());
                particles[n*DEPTH*HEIGHT+m*HEIGHT+i]=pos;
                // clog << "particle " << n*DEPTH*HEIGHT+m*HEIGHT+i << " is at " << particles[n*DEPTH*HEIGHT+m*HEIGHT+i] << endl;
            }
        }
    }
    clog << "Done." << endl;
    clog << "The " << WIDTH << "*" << DEPTH << "*" << HEIGHT << " box contains " << Npart << " particles." << endl;
 
    /// create a graph for KMC
    clog << "Making a new KMC graph." << endl;
    graph kmc_grid;

    clog << "Adding vertices." << endl;
    for (unsigned int i = 0; i < Npart ; ++i){
        kmc_grid.AddVertex(particles[i], field);
    }
    
    // kmc_grid.print_vertices();

    clog << "Finding edges. ";
    for (unsigned int i = 0; i < Npart; i++){
        vec r_i = particles[i];
        for (unsigned int j = 0; j < Npart; j++){
            if(i!=j){
                vec r_ij = CalcDist(r_i, particles[j], pbc);
                if(abs(r_ij)<r_c && do_cont==false){
                    Js=J+Random::rand_gaussian(sigma);
                    w_f = sqrt(pi/(reorg*kT))*Js*Js*exp(-(dG+reorg)*(dG+reorg)/(4*kT*reorg))/hbar_eVs; // SI rate
                    w_b = sqrt(pi/(reorg*kT))*Js*Js*exp(-(-dG+reorg)*(-dG+reorg)/(4*kT*reorg))/hbar_eVs;
                    w_n = sqrt(pi/(reorg*kT))*Js*Js*exp(-(reorg)*(reorg)/(4*kT*reorg))/hbar_eVs;
                    if(abs(r_i.getZ()-particles[j].getZ())>(d_bohr*50)){continue;} // forbids jumping across z-pbc
                    else if(abs(r_ij.getZ())<abs(r_ij.getX())){
                        kmc_grid.AddEdge(i,j,w_n,r_ij);
                        // clog << " rate = " << rate_neut << endl;
                    }
                    else if(abs(r_ij.getZ())<abs(r_ij.getY())){
                        kmc_grid.AddEdge(i,j,w_n,r_ij);
                        // clog << " rate = " << rate_neut << endl;
                    }
                    else {
                        if(r_ij.getZ()<0){
                            kmc_grid.AddEdge(i,j,w_b,r_ij);
                            // clog << " rate = " << rate_neg << endl;
                        }
                        if(r_ij.getZ()>0){
                            kmc_grid.AddEdge(i,j,w_f,r_ij);
                            // clog << " rate = " << rate << endl;
                        }
                    }
                }
                else if (abs(r_ij)<r_c && do_cont==true){
                    Js=J+Random::rand_gaussian(sigma);
                    w_f = sqrt(pi/(reorg*kT))*Js*Js*exp(-(dG+reorg)*(dG+reorg)/(4*kT*reorg))/hbar_eVs; // SI rate
                    w_b = sqrt(pi/(reorg*kT))*Js*Js*exp(-(-dG+reorg)*(-dG+reorg)/(4*kT*reorg))/hbar_eVs;
                    w_n = sqrt(pi/(reorg*kT))*Js*Js*exp(-(reorg)*(reorg)/(4*kT*reorg))/hbar_eVs;
                    if(abs(r_ij.getZ())<abs(r_ij.getX())){
                        kmc_grid.AddEdge(i,j,w_n,r_ij);
                        // clog << " rate = " << rate_neut << endl;
                    }
                    else if(abs(r_ij.getZ())<abs(r_ij.getY())){
                        kmc_grid.AddEdge(i,j,w_n,r_ij);
                        // clog << " rate = " << rate_neut << endl;
                    }
                    else {
                        if(r_ij.getZ()<0){
                            kmc_grid.AddEdge(i,j,w_b,r_ij);
                            // clog << " rate = " << rate_neg << endl;
                        }
                        if(r_ij.getZ()>0){
                            kmc_grid.AddEdge(i,j,w_f,r_ij);
                            // clog << " rate = " << rate << endl;
                        }
                    }
                }
            }
        }
    }
    clog << "Done." << endl;
    
    //kmc_grid.print_neighbours();
    // kmc_grid.print_vertices();
    
    kmc_grid.SetField(field);
    kmc_grid.SetTotDist(HEIGHT*abs(c)); // total distance in Bohr
    clog << "Total distance = " << kmc_grid.getTotDist() << " [Bohr]" << endl;
    
    if(do_cont==true){
        kmc_grid.setGeneratorsOnly();
        clog << "Setting only generators." << endl;
    }
    else{
        kmc_grid.setGenerators();
        cout << "Setting generators and collectors." << endl;
    }
    
    // kmc_grid.printGenerators(); exit(0);
    // kmc_grid.printCollectors();
    // kmc_grid.print_neighbours();
    
    /// run KMC algorithm
    hoppers charges(&kmc_grid);
    charges.setRecordOcc(true);
    clog << "Initializing KMC algorithm." << endl;
    double t_tot = 0;
    if(do_cont==true){t_tot=1E-8;}
    else{t_tot=1E-6;}
    KMCAlg kmc_alg(t_tot, 1E-15, 1.2, 1, &charges);
    clog << "Starting run. Total time = " << t_tot << endl;
    if(do_cont==true){kmc_alg.kmcPBC(2);}
    else{kmc_alg.kmcNoZPBC(1E-3, 1000);}
    clog << "Completion successful." << endl;
    
    return (EXIT_SUCCESS);
}

vec CalcDist(const vec& r_i, const vec& r_j, const matrix& box){
    vec r_tp, r_dp, r_sp, r_ij;
    vec a = box.getCol(0); vec b = box.getCol(1); vec c = box.getCol(2);
    r_tp = r_j - r_i;
    r_dp = r_tp - c*round(r_tp.getZ()/c.getZ());  
    r_sp = r_dp - b*round(r_dp.getY()/b.getY());
    r_ij = r_sp - a*round(r_sp.getX()/a.getX());
    #ifdef DEBUG
    cout << "r_ij: " << r_ij << " r_tp: " << r_tp << " d: " << abs(r_ij) << "\n";
    #endif
    return r_ij;
}

// check conditions for use of gmx algorithm for triclinic pbc
void CheckInput(double _cutoff, const matrix& box){
    vec a = box.getCol(0); vec b = box.getCol(1); vec c = box.getCol(2);
    if(_cutoff > 0.5*abs(c) || _cutoff > 0.5*abs(b) || _cutoff > 0.5*abs(a))
        cerr << "Cut-off too large. Violates minimum image convention. \n";
    if(a.getY() != 0 || a.getZ() != 0 || b.getZ() != 0)
        cerr << "Equation (3.1) from Gromacs Manual not fulfilled. Check your box. \n"
             << a.getY() << " " << a.getZ() << " " << b.getZ() << "\n";
    if(a.getX() <= 0 || b.getY() <= 0 || c.getZ() <= 0)
        cerr << "Equation (3.2) from Gromacs Manual not fulfilled. Check your box. \n";
    if(b.getX() > 0.5*a.getX() || b.getX() < -0.5*a.getX())
        cerr << "Equation (3.3) from Gromacs Manual not fulfilled. Check your box. \n";
    if(c.getX() > 0.5*a.getX() || c.getX() < -0.5*a.getX())
        cerr << "Equation (3.3) from Gromacs Manual not fulfilled. Check your box. \n";
    if(c.getY() > 0.5*b.getY() || c.getY() < -0.5*b.getY())
        cerr << "Equation (3.3) from Gromacs Manual not fulfilled. Check your box. \n";
}

