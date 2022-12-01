//2d gas simulation for brownian motion
//Method: Finite elements + Omelyan Position-Extended-Forest-Ruth-Like (PEFRL)
//Author: davidsantiagoquevedo
//Created on Mon Oct 17 2022
//Vector.h and Random64.h were developed by: 
//Prof. Dr. JD Muñoz, Universidad Nacional de Colombia (http://www.hermes.unal.edu.co/pages/Consultas/Grupo.xhtml?idGrupo=797&opcion=1)

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include "Vector.h"
#include "Random64.h"
using namespace std;

// System constants
const double Lx = 60, Ly = 60;
const int Nx = 10, Ny = 10, N = Nx*Ny;
const int N_brown_x = 5,  N_brown_y = 5;
const int N_times = 100;
// PEFRL constants
const double Zi = 0.1786178958448091e0;
const double Lambda = 0.2123418310626054*(-1);
const double Xi = 0.06626458266981849*(-1);
const double Coef1 = (1-2*Lambda) / 2;
const double Coef2 = (1-2*(Xi+Zi));

// Classes
class body;
class colission;
class random_walk;

//---------
// body
class body{
private:
  vector3D r, V, F; 
  double m, R;
public:
  void initiate(double x0, double y0, double z0,
                double Vx0, double Vy0, double Vz0,
                double m0, double R0);
  void reset_F(void){F.cargue(0,0,0);};
  void add_F(vector3D dF){F += dF;};
  void update_r(double dt,double coef);
  void update_V(double dt,double coef);
  void render(void);
  void get_positions(void);
  void get_features(void);
  double Getx(void){return r.x();}; //Inline
  double Gety(void){return r.y();}; //Inline
  double Getz(void){return r.z();}; //Inline
  double GetVx(void){return V.x();}; //Inline
  double GetVy(void){return V.y();}; //Inline
  double GetVz(void){return V.z();}; //Inline
  friend class colission;
  friend class simulation;
  friend class random_walk;
};
void body::initiate(double x0,double y0,double z0,
                    double Vx0,double Vy0,double Vz0,
                    double m0,double R0){
  r.cargue(x0,y0,z0); 
  V.cargue(Vx0,Vy0,Vz0); 
  m=m0; 
  R=R0;
}
void body::update_r(double dt,double coef){
  r+=V*(dt*coef);
}
void body::update_V(double dt, double coef){
  V+=F*(dt*coef/m);
}
void body::render(void){
  cout<<" , "<<r.x()<<"+"<<R<<"*cos(t),"<<r.y()<<"+"<<R<<"*sin(t)";
}
void body::get_positions(void){
  cout<<r.x()<<","<<r.y();
}
void body::get_features(void){
  cout<<m<<","<<R;
}

//------------
// random walk
class random_walk{
private:
  vector3D r, V;
public:
  void return_data(body & molecule);
  void render(void);
  friend class body;
};
void random_walk::return_data(body & molecule){
  r.cargue(molecule.Getx(), molecule.Gety(), molecule.Getz());
  V.cargue(molecule.GetVx(), molecule.GetVy(), molecule.GetVz());
}
void random_walk::render(void){
  cout<<r.x()<<","<<r.y()<<","<<V.x()<<","<<V.y();
}

//----------
// colission
class colission{
private:
  const double K = 1.0e4;
public:
  void force_pairwise(body & molecule1, body & molecule2);
  void force_ensemble(body * molecule); 
  friend class simulation;
};
void colission::force_pairwise(body & molecule1, body & molecule2){
  vector3D dr = molecule2.r-molecule1.r; 
  double Norma_dr = norma(dr);
  double s = (molecule2.R+molecule1.R)-Norma_dr;
  if(s>0){//colission occurs
    vector3D F2=dr*(K*pow(s,1.5) / Norma_dr);
  molecule2.add_F(F2);  molecule1.add_F(F2*(-1));
}
}
void colission::force_ensemble(body*molecule){
  int i,j;
  for(i = 0; i < (N+4); i++) molecule[i].reset_F();
  for(i = 0; i < N; i++)
    for(j = i+1; j < (N+4); j++)
      force_pairwise(molecule[i], molecule[j]);
}

//-----------------
// output functions
void start_animation(void){
  cout<<"set terminal gif animate"<<endl; 
  cout<<"set output filename"<<endl;
  cout<<"unset key"<<endl;
  cout<<"set xrange[-10:"<<Lx+10<<"]"<<endl;
  cout<<"set yrange[-10:"<<Ly+10<<"]"<<endl;
  cout<<"set size ratio -1"<<endl;
  cout<<"set parametric"<<endl;
  cout<<"set trange [0:7]"<<endl;
  cout<<"set isosamples 12"<<endl;  
}
void start_frame(void){
    cout<<"plot 0,0 ";
    cout<<" , "<<Lx/7<<"*t,0";          //lower wall
    cout<<" , "<<Lx/7<<"*t,"<<Ly;       //upper wall
    cout<<" , 0,"<<Ly/7<<"*t";          //left wall
    cout<<" , "<<Lx<<","<<Ly/7<<"*t";   //right wall
}
void end_frame(void){
    cout<<endl;
}

int main(int argc, char **argv){
  char* output = argv[1];
  double T = 1;
  double t, t_draw, dt = 1.0e-3;
  
  // Objects involved in the simulation
  body molecules[N+4]; //Molecules: walls are the last four grains
  colission Newton; //Colission object
  Crandom ran64(1); //Random number generator
  random_walk step_n; //Instantaneous state of brownian molecule (nth step of the random walk)

  // Physical constants
  // Gas molecules:
  double m0 = 1, R0 = 2;
  double kT = 1000;
  double dx = Lx/(Nx+1), dy = Ly/(Ny+1), x0, y0; //lattice constantes
  double V0 = sqrt(2*kT/m0), theta, Vx0, Vy0;
  // Walls:
  double m_wall = 100*m0, R_wall = 10000;
  // Brownian Molecule:
  double m_brownian = 3*m0, R_brownian = 3*R0;
  double V0_brownian = sqrt(2*kT/m_brownian);

  // Iterators:
  int i, j;
  
  //---------------
  //Initiate system
  //WALLS
  //Upper wall
  molecules[N  ].initiate(Lx/2,Ly+R_wall,0,0,0,0,m_wall,R_wall);
  //Lower wall
  molecules[N+1].initiate(Lx/2,  -R_wall,0,0,0,0,m_wall,R_wall);
  //Right wall
  molecules[N+2].initiate(Lx+R_wall,Ly/2,0,0,0,0,m_wall,R_wall);
  //Left wall
  molecules[N+3].initiate(  -R_wall,Ly/2,0,0,0,0,m_wall,R_wall);
  
  //Molecules
  for(i = 0; i < Nx; i++)
    for(j = 0;j < Ny; j++){
      x0 = (i+1)*dx; 
      y0 = (j+1)*dy;
      theta = 2*M_PI*ran64.r(); 
      if(i == N_brown_x & j == N_brown_y){ //Brownian molecule
        Vx0 = V0_brownian*cos(theta);
        Vy0 = V0_brownian*sin(theta);
        molecules[i*Ny + j].initiate(x0, y0, 0,  Vx0, Vy0,  0, m_brownian, R_brownian);
      }else{ // Gas molecules
        Vx0 = V0*cos(theta);
        Vy0 = V0*sin(theta);
        molecules[i*Ny+j].initiate(x0, y0, 0,  Vx0, Vy0,  0, m0, R0);
      }
    }

  //---------------
  // Simulation
  if(strcmp ("a", output) == 0){
    start_animation();
  }
  for(t= t_draw = 0; t < N_times*T; t += dt, t_draw += dt){
    body brownian_molecule = molecules[N_brown_x*Ny + N_brown_y];
    step_n.return_data(brownian_molecule);
    if(strcmp ("r", output) == 0){
      step_n.render();
      end_frame();
    }
    if(t_draw > T/100){
      if(strcmp ("a", output) == 0){
        if(t < 1000*dt){
          start_frame();
          for(i = 0; i < N; i++) molecules[i].render();
          end_frame();
          t_draw = 0;
        }
      }
      if(strcmp ("e", output) == 0){
        for(i = 0; i < N; i++) molecules[i].get_positions();
        cout<<N_brown_x*Ny + N_brown_y<<',';
        cout<<R_brownian<<',';
        cout<<m_brownian<<',';
        cout<<R0<<',';
        cout<<m0<<',';
        end_frame();
        t_draw = 0;
      }
    }
    //Fourth order PEFRL
    for(i = 0; i < N; i++) molecules[i].update_r(dt, Zi);
    Newton.force_ensemble(molecules);
    for(i = 0; i < N; i++) molecules[i].update_V(dt, Coef1);
    for(i = 0; i < N; i++) molecules[i].update_r(dt, Xi);
    Newton.force_ensemble(molecules);
    for(i = 0; i < N; i++) molecules[i].update_V(dt, Lambda);
    for(i = 0; i < N; i++) molecules[i].update_r(dt, Coef2);
    Newton.force_ensemble(molecules);
    for(i = 0; i < N; i++) molecules[i].update_V(dt, Lambda);
    for(i = 0;i < N; i++) molecules[i].update_r(dt, Xi);
    Newton.force_ensemble(molecules);
    for(i = 0; i < N; i++) molecules[i].update_V(dt, Coef1);
    for(i = 0; i < N; i++) molecules[i].update_r(dt, Zi);
  }
  return 0;
}
