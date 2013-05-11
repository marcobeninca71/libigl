#include "launch_medit.h"
#include "writeMESH.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>

#define MEDIT_PATH "/opt/local/bin/medit"
#define TEMP_MESH_FILE "/var/tmp/temp.mesh"
#define TEMP_MEDIT_FILE "/var/tmp/temp.medit"

template <typename DerivedV, typename DerivedT, typename DerivedF>
IGL_INLINE int igl::launch_medit(
  const Eigen::PlainObjectBase<DerivedV> & V, 
  const Eigen::PlainObjectBase<DerivedT> & T,
  const Eigen::PlainObjectBase<DerivedF> & F,
  const bool wait)
{
  using namespace std;
  // Build medit command, end with & so command returns without waiting
  stringstream command; 
  command<<MEDIT_PATH<<" "<<TEMP_MESH_FILE<<" "<<TEMP_MEDIT_FILE;
  if(!wait)
  {
    command<<" &";
  }
  bool mesh_saved = writeMESH(TEMP_MESH_FILE,V,T,F);
  if(!mesh_saved)
  {
    return -1;
  }
  // Write default medit options
  const string default_medit_file_contents = 
    "BackgroundColor 1 1 1\n"
    "LineColor 0 0 0\n"
    "WindowSize 1024 800\n"
    "RenderMode shading + lines\n";
  FILE * fp = fopen(TEMP_MEDIT_FILE,"w");
  if(fp == NULL)
  {
    cerr<<"^"<<__FUNCTION__<<": Could not write to "<<TEMP_MEDIT_FILE<<endl;
    return -1;
  }
  fprintf(fp,"%s",default_medit_file_contents.c_str());
  fclose(fp);

  try
  {
    return system(command.str().c_str());
  }catch(int e)
  {
    cerr<<"^"<<__FUNCTION__<<": Calling to medit crashed..."<<endl;
    return -1;
  }
  // Could clean up and delete these files but not really worth it
}

#ifndef IGL_HEADER_ONLY
// Explicit template specialization
template int igl::launch_medit<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, bool);
#endif

