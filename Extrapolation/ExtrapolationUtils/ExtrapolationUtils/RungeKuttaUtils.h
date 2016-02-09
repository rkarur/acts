/////////////////////////////////////////////////////////////////////////////////
//  Header file for class  RungeKuttaUtils, ATS project
//  author Igor Gavrilenko     
/////////////////////////////////////////////////////////////////////////////////

#ifndef RungeKuttaUtils_H
#define RungeKuttaUtils_H

#include <vector>
#include <map>
#include "GaudiKernel/AlgTool.h"
#include "TrackParameters/TrackParameters.h"
#include "NeutralParameters/NeutralParameters.h"
#include "Surfaces/BoundaryCheck.h"

namespace Ats {

  class ConeSurface           ;
  class DiscSurface           ;
  class PlaneSurface          ;
  class CylinderSurface       ;
  class StraightLineSurface   ;
#ifndef ATS_GAUDI_BUILD
  class PatternTrackParameters;
#endif
  /**
  @class RungeKuttaUtils
<<<<<<< HEAD:Tracking/TrkExtrapolation/TrkExUtils/TrkExUtils/RungeKuttaUtils.h

  Trk::RungeKuttaUtils is set algorithms for track parameters transformation
=======
  
  Ats::RungeKuttaUtils is set algorithms for track parameters transformation
>>>>>>> origin/acts-packaging:Extrapolation/ExtrapolationUtils/ExtrapolationUtils/RungeKuttaUtils.h
  from local to global and global to local system coordinate and step
  estimation to surface.

    AtaPlane    AtaStraightLine      AtaDisc       AtaCylinder      Perigee
       |               |               |               |              |
       |               |               |               |              |
       V               V               V               V              V
       -----------------------------------------------------------------
                                       |              Local->Global transformation
                                       V
                    Global position (Runge Kutta presentation)
                                       |
                                       V              Global->Local transformation
       ----------------------------------------------------------------
       |               |               |               |              |
       |               |               |               |              |
       V               V               V               V              V
   PlaneSurface StraightLineSurface DiscSurface CylinderSurface PerigeeSurface

  For using Runge Kutta method we use global coordinate, direction,
  inverse momentum and Jacobian of transformation. All this parameters we save
  in array P[42].
                   /dL0    /dL1    /dPhi   /dThe   /dCM
  X  ->P[0]  dX /   P[ 7]   P[14]   P[21]   P[28]   P[35]
  Y  ->P[1]  dY /   P[ 8]   P[15]   P[22]   P[29]   P[36]
  Z  ->P[2]  dZ /   P[ 9]   P[16]   P[23]   P[30]   P[37]
  Ax ->P[3]  dAx/   P[10]   P[17]   P[24]   P[31]   P[38]
  Ay ->P[4]  dAy/   P[11]   P[18]   P[25]   P[32]   P[39]
  Az ->P[5]  dAz/   P[12]   P[19]   P[26]   P[33]   P[40]
  CM ->P[6]  dCM/   P[13]   P[20]   P[27]   P[34]   P[41]

  where
       in case local presentation

       L0  - first  local coordinate  (surface dependent)
       L1  - second local coordinate  (surface dependent)
       Phi - Azimuthal angle
       The - Polar     angle
       CM  - charge/momentum

       in case global presentation

       X   - global x-coordinate        = surface dependent
       Y   - global y-coordinate        = surface dependent
       Z   - global z-coordinate        = sutface dependent
       Ax  - direction cosine to x-axis = Sin(The)*Cos(Phi)
       Ay  - direction cosine to y-axis = Sin(The)*Sin(Phi)
       Az  - direction cosine to z-axis = Cos(The)
       CM  - charge/momentum            = local CM

  @author Igor.Gavrilenko@cern.ch
  **/

  class RungeKuttaUtils
    {
      /////////////////////////////////////////////////////////////////////////////////
      // Public methods:
      /////////////////////////////////////////////////////////////////////////////////

    public:

      RungeKuttaUtils(){};
      virtual ~RungeKuttaUtils (){};

      /////////////////////////////////////////////////////////////////////////////////
      // Step estimators to surface
      /////////////////////////////////////////////////////////////////////////////////

      double stepEstimator              (int,double*,const double*,bool&) const;
      double stepEstimatorToCone            (double*,const double*,bool&) const;
      double stepEstimatorToPlane           (double*,const double*,bool&) const;
      double stepEstimatorToCylinder        (double*,const double*,bool&) const;
      double stepEstimatorToStraightLine    (double*,const double*,bool&) const;

      /////////////////////////////////////////////////////////////////////////////////
      // Step estimators to surfaces
      /////////////////////////////////////////////////////////////////////////////////
      std::pair<double,int> stepEstimator 
	(std::vector<std::pair<const Ats::Surface*,Ats::BoundaryCheck> >&,
	 std::multimap<double,int>&,
	 const double*,const double*,double,double,int Nv,bool&) const;

      int fillDistancesMap
	(std::vector<std::pair<const Ats::Surface*,Ats::BoundaryCheck> >&,
	 std::multimap<double,int>&,
	 const double*,double,const Ats::Surface*,double*) const;
      
      /////////////////////////////////////////////////////////////////////////////////
      // Transformations from local to global system coordinates 
      // for Ats::TrackParameters and Ats::NeutralParameters 
      /////////////////////////////////////////////////////////////////////////////////

      bool transformLocalToGlobal
	(bool,const Ats::TrackParameters&        ,double*) const;

      bool transformLocalToGlobal
	(bool,const Ats::NeutralParameters&       ,double*) const;

      /////////////////////////////////////////////////////////////////////////////////
      // Transformations from local to global system coordinates 
      // for Ats::PatternTrackParameters
      /////////////////////////////////////////////////////////////////////////////////
#ifndef ATS_GAUDI_BUILD
      bool transformLocalToGlobal
	(bool,const Ats::PatternTrackParameters&        ,double*) const;
#endif
      /////////////////////////////////////////////////////////////////////////////////
      // Transformations from local to global system coordinates
      // for different surfaces
      /////////////////////////////////////////////////////////////////////////////////

      void transformDiscToGlobal        
	(bool,const Ats::Surface*,const double*,double*) const;
      void transformPlaneToGlobal       
	(bool,const Ats::Surface*,const double*,double*) const;
      void transformCylinderToGlobal    
	(bool,const Ats::Surface*,const double*,double*) const;
      void transformLineToGlobal
	(bool,const Ats::Surface*,const double*,double*) const;

      /////////////////////////////////////////////////////////////////////////////////
      // Transformations from global to local system coordinates
      /////////////////////////////////////////////////////////////////////////////////

      void transformGlobalToLocal        (double*,double*) const;
      void transformGlobalToLocal
	 (const Ats::Surface*,bool,double*,double*,double*) const;
      void transformGlobalToCone
	(const Ats::Surface*,bool,double*,double*,double*) const;
      void transformGlobalToDisc
	(const Ats::Surface*,bool,double*,double*,double*) const;
      void transformGlobalToPlane
	(const Ats::Surface*,bool,double*,double*,double*) const;
      void transformGlobalToCylinder
  	(const Ats::Surface*,bool,double*,double*,double*) const;
      void transformGlobalToLine
	(const Ats::Surface*,bool,double*,double*,double*) const;

     /////////////////////////////////////////////////////////////////////////////////

      /////////////////////////////////////////////////////////////////////////////////
      // Covariance matrix production for Ats::TrackParameters
      /////////////////////////////////////////////////////////////////////////////////
      
      AtsSymMatrixD<5>*  newCovarianceMatrix
	(double*,const AtsSymMatrixD<5>&) const;

      /////////////////////////////////////////////////////////////////////////////////
      // Transformations from curvilinear to global system coordinates
      // covariance matrix only
      /////////////////////////////////////////////////////////////////////////////////

      void transformCurvilinearToGlobal
	(double* ,double*) const;

      /////////////////////////////////////////////////////////////////////////////////
      // Transformations from global to curvilinear system coordinates
      // covariance matrix only
      /////////////////////////////////////////////////////////////////////////////////

      void transformGlobalToCurvilinear
	(bool,double*,double*,double*) const;

      /////////////////////////////////////////////////////////////////////////////////
      // Jacobian of transformations from curvilinear to local system coordinates
      /////////////////////////////////////////////////////////////////////////////////

      void jacobianTransformCurvilinearToLocal
	(const Ats::TrackParameters&,double*);
#ifndef ATS_GAUDI_BUILD
      void jacobianTransformCurvilinearToLocal
	(const Ats::PatternTrackParameters&,double*); 
#endif
      void jacobianTransformCurvilinearToLocal
	(double*,const Ats::Surface*,double*);
   
      void jacobianTransformCurvilinearToDisc        (double*,double*) const;
      void jacobianTransformCurvilinearToPlane       (double*,double*) const;
      void jacobianTransformCurvilinearToCylinder    (double*,double*) const;
      void jacobianTransformCurvilinearToStraightLine(double*,double*) const;

    private:

      /////////////////////////////////////////////////////////////////////////////////
      // Private methods:
      /////////////////////////////////////////////////////////////////////////////////

      bool transformLocalToGlobal(bool,const Ats::Surface*,const double*,double*) const;
    };

}

#endif // RungeKuttaUtils_H
