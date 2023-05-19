/* Created by Language version: 7.7.0 */
/* NOT VECTORIZED */
#define NRN_VECTORIZED 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mech_api.h"
#undef PI
#define nil 0
#include "md1redef.h"
#include "section.h"
#include "nrniv_mf.h"
#include "md2redef.h"
 
#if METHOD3
extern int _method3;
#endif

#if !NRNGPU
#undef exp
#define exp hoc_Exp
extern double hoc_Exp(double);
#endif
 
#define nrn_init _nrn_init__AmpaSyn
#define _nrn_initial _nrn_initial__AmpaSyn
#define nrn_cur _nrn_cur__AmpaSyn
#define _nrn_current _nrn_current__AmpaSyn
#define nrn_jacob _nrn_jacob__AmpaSyn
#define nrn_state _nrn_state__AmpaSyn
#define _net_receive _net_receive__AmpaSyn 
#define state state__AmpaSyn 
 
#define _threadargscomma_ /**/
#define _threadargsprotocomma_ /**/
#define _threadargs_ /**/
#define _threadargsproto_ /**/
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *getarg();
 static double *_p; static Datum *_ppvar;
 
#define t nrn_threads->_t
#define dt nrn_threads->_dt
#define e _p[0]
#define e_columnindex 0
#define i _p[1]
#define i_columnindex 1
#define ica _p[2]
#define ica_columnindex 2
#define g _p[3]
#define g_columnindex 3
#define A _p[4]
#define A_columnindex 4
#define B _p[5]
#define B_columnindex 5
#define C _p[6]
#define C_columnindex 6
#define tmax _p[7]
#define tmax_columnindex 7
#define cai _p[8]
#define cai_columnindex 8
#define cao _p[9]
#define cao_columnindex 9
#define DA _p[10]
#define DA_columnindex 10
#define DB _p[11]
#define DB_columnindex 11
#define DC _p[12]
#define DC_columnindex 12
#define Dtmax _p[13]
#define Dtmax_columnindex 13
#define _g _p[14]
#define _g_columnindex 14
#define _tsav _p[15]
#define _tsav_columnindex 15
#define _nd_area  *_ppvar[0]._pval
#define _ion_cai	*_ppvar[2]._pval
#define _ion_cao	*_ppvar[3]._pval
#define _ion_ica	*_ppvar[4]._pval
#define _ion_dicadv	*_ppvar[5]._pval
 
#if MAC
#if !defined(v)
#define v _mlhv
#endif
#if !defined(h)
#define h _mlhh
#endif
#endif
 
#if defined(__cplusplus)
extern "C" {
#endif
 static int hoc_nrnpointerindex =  -1;
 /* external NEURON variables */
 extern double celsius;
 /* declaration of user functions */
 static double _hoc_ghk(void*);
 static double _hoc_ghkg(void*);
 static int _mechtype;
extern void _nrn_cacheloop_reg(int, int);
extern void hoc_register_prop_size(int, int, int);
extern void hoc_register_limits(int, HocParmLimits*);
extern void hoc_register_units(int, HocParmUnits*);
extern void nrn_promote(Prop*, int, int);
extern Memb_func* memb_func;
 
#define NMODL_TEXT 1
#if NMODL_TEXT
static const char* nmodl_file_text;
static const char* nmodl_filename;
extern void hoc_reg_nmodl_text(int, const char*);
extern void hoc_reg_nmodl_filename(int, const char*);
#endif

 extern Prop* nrn_point_prop_;
 static int _pointtype;
 static void* _hoc_create_pnt(Object* _ho) { void* create_point_process(int, Object*);
 return create_point_process(_pointtype, _ho);
}
 static void _hoc_destroy_pnt(void*);
 static double _hoc_loc_pnt(void* _vptr) {double loc_point_process(int, void*);
 return loc_point_process(_pointtype, _vptr);
}
 static double _hoc_has_loc(void* _vptr) {double has_loc_point(void*);
 return has_loc_point(_vptr);
}
 static double _hoc_get_loc_pnt(void* _vptr) {
 double get_loc_point_process(void*); return (get_loc_point_process(_vptr));
}
 extern void _nrn_setdata_reg(int, void(*)(Prop*));
 static void _setdata(Prop* _prop) {
 _p = _prop->param; _ppvar = _prop->dparam;
 }
 static void _hoc_setdata(void* _vptr) { Prop* _prop;
 _prop = ((Point_process*)_vptr)->_prop;
   _setdata(_prop);
 }
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 0,0
};
 static Member_func _member_func[] = {
 "loc", _hoc_loc_pnt,
 "has_loc", _hoc_has_loc,
 "get_loc", _hoc_get_loc_pnt,
 "ghk", _hoc_ghk,
 "ghkg", _hoc_ghkg,
 0, 0
};
#define ghk ghk_AmpaSyn
#define ghkg ghkg_AmpaSyn
 extern double ghk( double , double , double , double );
 extern double ghkg( double , double , double , double );
 /* declare global and static user variables */
#define T_exp T_exp_AmpaSyn
 double T_exp = 22;
#define aslow aslow_AmpaSyn
 double aslow = 0;
#define afast afast_AmpaSyn
 double afast = 0.75;
#define fracca fracca_AmpaSyn
 double fracca = 0.002;
#define normfac normfac_AmpaSyn
 double normfac = 0;
#define q10 q10_AmpaSyn
 double q10 = 3;
#define tauslow_exp tauslow_exp_AmpaSyn
 double tauslow_exp = 2.55;
#define taufast_exp taufast_exp_AmpaSyn
 double taufast_exp = 0.61;
#define taurise_exp taurise_exp_AmpaSyn
 double taurise_exp = 0.2;
#define tauslow tauslow_AmpaSyn
 double tauslow = 0;
#define taufast taufast_AmpaSyn
 double taufast = 0;
#define taurise taurise_AmpaSyn
 double taurise = 0;
#define total total_AmpaSyn
 double total = 0;
#define z z_AmpaSyn
 double z = 2;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "afast_AmpaSyn", 0, 1,
 "tauslow_exp_AmpaSyn", 1e-09, 1e+09,
 "taufast_exp_AmpaSyn", 1e-09, 1e+09,
 "taurise_exp_AmpaSyn", 1e-09, 1e+09,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "taurise_exp_AmpaSyn", "ms",
 "taufast_exp_AmpaSyn", "ms",
 "tauslow_exp_AmpaSyn", "ms",
 "T_exp_AmpaSyn", "degC",
 "total_AmpaSyn", "uS",
 "taurise_AmpaSyn", "ms",
 "taufast_AmpaSyn", "ms",
 "tauslow_AmpaSyn", "ms",
 "e", "mV",
 "A", "uS",
 "B", "uS",
 "C", "uS",
 "tmax", "ms",
 "i", "nA",
 "ica", "nA",
 "g", "uS",
 0,0
};
 static double A0 = 0;
 static double B0 = 0;
 static double C0 = 0;
 static double delta_t = 0.01;
 static double tmax0 = 0;
 static double v = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "taurise_exp_AmpaSyn", &taurise_exp_AmpaSyn,
 "taufast_exp_AmpaSyn", &taufast_exp_AmpaSyn,
 "tauslow_exp_AmpaSyn", &tauslow_exp_AmpaSyn,
 "afast_AmpaSyn", &afast_AmpaSyn,
 "fracca_AmpaSyn", &fracca_AmpaSyn,
 "z_AmpaSyn", &z_AmpaSyn,
 "T_exp_AmpaSyn", &T_exp_AmpaSyn,
 "q10_AmpaSyn", &q10_AmpaSyn,
 "aslow_AmpaSyn", &aslow_AmpaSyn,
 "total_AmpaSyn", &total_AmpaSyn,
 "taurise_AmpaSyn", &taurise_AmpaSyn,
 "taufast_AmpaSyn", &taufast_AmpaSyn,
 "tauslow_AmpaSyn", &tauslow_AmpaSyn,
 "normfac_AmpaSyn", &normfac_AmpaSyn,
 0,0
};
 static DoubVec hoc_vdoub[] = {
 0,0,0
};
 static double _sav_indep;
 static void nrn_alloc(Prop*);
static void  nrn_init(NrnThread*, _Memb_list*, int);
static void nrn_state(NrnThread*, _Memb_list*, int);
 static void nrn_cur(NrnThread*, _Memb_list*, int);
static void  nrn_jacob(NrnThread*, _Memb_list*, int);
 static void _hoc_destroy_pnt(void* _vptr) {
   destroy_point_process(_vptr);
}
 
static int _ode_count(int);
static void _ode_map(int, double**, double**, double*, Datum*, double*, int);
static void _ode_spec(NrnThread*, _Memb_list*, int);
static void _ode_matsol(NrnThread*, _Memb_list*, int);
 
#define _cvode_ieq _ppvar[6]._i
 static void _ode_matsol_instance2(_threadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"AmpaSyn",
 "e",
 0,
 "i",
 "ica",
 "g",
 0,
 "A",
 "B",
 "C",
 "tmax",
 0,
 0};
 static Symbol* _ca_sym;
 
extern Prop* need_memb(Symbol*);

static void nrn_alloc(Prop* _prop) {
	Prop *prop_ion;
	double *_p; Datum *_ppvar;
  if (nrn_point_prop_) {
	_prop->_alloc_seq = nrn_point_prop_->_alloc_seq;
	_p = nrn_point_prop_->param;
	_ppvar = nrn_point_prop_->dparam;
 }else{
 	_p = nrn_prop_data_alloc(_mechtype, 16, _prop);
 	/*initialize range parameters*/
 	e = 0;
  }
 	_prop->param = _p;
 	_prop->param_size = 16;
  if (!nrn_point_prop_) {
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 7, _prop);
  }
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[2]._pval = &prop_ion->param[1]; /* cai */
 	_ppvar[3]._pval = &prop_ion->param[2]; /* cao */
 	_ppvar[4]._pval = &prop_ion->param[3]; /* ica */
 	_ppvar[5]._pval = &prop_ion->param[4]; /* _ion_dicadv */
 
}
 static void _initlists();
  /* some states have an absolute tolerance */
 static Symbol** _atollist;
 static HocStateTolerance _hoc_state_tol[] = {
 0,0
};
 static void _net_receive(Point_process*, double*, double);
 static void _update_ion_pointer(Datum*);
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
extern void _nrn_thread_table_reg(int, void(*)(double*, Datum*, Datum*, NrnThread*, int));
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 void _ampa_forti_reg() {
	int _vectorized = 0;
  _initlists();
 	ion_reg("ca", -10000.);
 	_ca_sym = hoc_lookup("ca_ion");
 	_pointtype = point_register_mech(_mechanism,
	 nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init,
	 hoc_nrnpointerindex, 0,
	 _hoc_create_pnt, _hoc_destroy_pnt, _member_func);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 2, _update_ion_pointer);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 16, 7);
  hoc_register_dparam_semantics(_mechtype, 0, "area");
  hoc_register_dparam_semantics(_mechtype, 1, "pntproc");
  hoc_register_dparam_semantics(_mechtype, 2, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 6, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 pnt_receive[_mechtype] = _net_receive;
 pnt_receive_size[_mechtype] = 1;
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 AmpaSyn ampa_forti.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 
#define FARADAY _nrnunit_FARADAY[_nrnunit_use_legacy_]
static double _nrnunit_FARADAY[2] = {0x1.78e555060882cp+16, 96485.3}; /* 96485.3321233100141 */
 
#define R _nrnunit_R[_nrnunit_use_legacy_]
static double _nrnunit_R[2] = {0x1.0a1013e8990bep+3, 8.3145}; /* 8.3144626181532395 */
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
 
static int _ode_spec2(_threadargsproto_);
/*static int _ode_matsol2(_threadargsproto_);*/
 static int _slist2[3], _dlist2[3];
 static int state(_threadargsproto_);
 static int _slist1[1]; static double _dlist1[1];
 static void peak();
 
static void peak ()
 {
    int _counte = -1;
 _dlist1[++_counte] =  -( 1.0 / taurise * exp ( - tmax / taurise ) - afast / taufast * exp ( - tmax / taufast ) - aslow / tauslow * exp ( - tmax / tauslow ) ) +  0.0 ;
   }
 
/*CVODE*/
 static int _ode_spec2 () {_reset=0;
 {
   DA = - A / taurise ;
   DB = - B / taufast ;
   DC = - C / tauslow ;
   }
 return _reset;
}
 static int _ode_matsol2 () {
 DA = DA  / (1. - dt*( ( - 1.0 ) / taurise )) ;
 DB = DB  / (1. - dt*( ( - 1.0 ) / taufast )) ;
 DC = DC  / (1. - dt*( ( - 1.0 ) / tauslow )) ;
  return 0;
}
 /*END CVODE*/
 static int state () {_reset=0;
 {
    A = A + (1. - exp(dt*(( - 1.0 ) / taurise)))*(- ( 0.0 ) / ( ( - 1.0 ) / taurise ) - A) ;
    B = B + (1. - exp(dt*(( - 1.0 ) / taufast)))*(- ( 0.0 ) / ( ( - 1.0 ) / taufast ) - B) ;
    C = C + (1. - exp(dt*(( - 1.0 ) / tauslow)))*(- ( 0.0 ) / ( ( - 1.0 ) / tauslow ) - C) ;
   }
  return 0;
}
 
static void _net_receive (Point_process* _pnt, double* _args, double _lflag) 
{    _p = _pnt->_prop->param; _ppvar = _pnt->_prop->dparam;
  if (_tsav > t){ extern char* hoc_object_name(); hoc_execerror(hoc_object_name(_pnt->ob), ":Event arrived out of order. Must call ParallelContext.set_maxstep AFTER assigning minimum NetCon.delay");}
 _tsav = t; {
   double _lnormweight ;
 _lnormweight = _args[0] / normfac ;
       if (nrn_netrec_state_adjust && !cvode_active_){
    /* discon state adjustment for cnexp case (rate uses no local variable) */
    double __state = A;
    double __primary = (A + _lnormweight ) - __state;
     __primary += ( 1. - exp( 0.5*dt*( ( - 1.0 ) / taurise ) ) )*( - ( 0.0 ) / ( ( - 1.0 ) / taurise ) - __primary );
    A += __primary;
  } else {
 A = A + _lnormweight  ;
     }
     if (nrn_netrec_state_adjust && !cvode_active_){
    /* discon state adjustment for cnexp case (rate uses no local variable) */
    double __state = B;
    double __primary = (B + _lnormweight * afast ) - __state;
     __primary += ( 1. - exp( 0.5*dt*( ( - 1.0 ) / taufast ) ) )*( - ( 0.0 ) / ( ( - 1.0 ) / taufast ) - __primary );
    B += __primary;
  } else {
 B = B + _lnormweight * afast  ;
     }
     if (nrn_netrec_state_adjust && !cvode_active_){
    /* discon state adjustment for cnexp case (rate uses no local variable) */
    double __state = C;
    double __primary = (C + _lnormweight * aslow ) - __state;
     __primary += ( 1. - exp( 0.5*dt*( ( - 1.0 ) / tauslow ) ) )*( - ( 0.0 ) / ( ( - 1.0 ) / tauslow ) - __primary );
    C += __primary;
  } else {
 C = C + _lnormweight * aslow  ;
     }
 total = total + _lnormweight ;
   } }
 
double ghkg (  double _lv , double _lci , double _lco , double _lz ) {
   double _lghkg;
 double _lxi , _lf , _lexi , _lfxi ;
 _lf = R * ( celsius + 273.15 ) / ( _lz * ( 1e-3 ) * FARADAY ) ;
   _lxi = _lv / _lf ;
   _lexi = exp ( _lxi ) ;
   if ( fabs ( _lxi ) < 1e-4 ) {
     _lfxi = 1.0 - _lxi / 2.0 ;
     }
   else {
     _lfxi = _lxi / ( _lexi - 1.0 ) ;
     }
   _lghkg = _lf * ( ( _lci / _lco ) * _lexi - 1.0 ) * _lfxi ;
   
return _lghkg;
 }
 
static double _hoc_ghkg(void* _vptr) {
 double _r;
    _hoc_setdata(_vptr);
 _r =  ghkg (  *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 return(_r);
}
 
double ghk (  double _lv , double _lci , double _lco , double _lz ) {
   double _lghk;
 double _lxi , _lf , _lexi , _lfxi ;
 _lf = R * ( celsius + 273.15 ) / ( _lz * ( 1e-3 ) * FARADAY ) ;
   _lxi = _lv / _lf ;
   _lexi = exp ( _lxi ) ;
   if ( fabs ( _lxi ) < 1e-4 ) {
     _lfxi = 1.0 - _lxi / 2.0 ;
     }
   else {
     _lfxi = _lxi / ( _lexi - 1.0 ) ;
     }
   _lghk = ( .001 ) * _lz * FARADAY * ( _lci * _lexi - _lco ) * _lfxi ;
   
return _lghk;
 }
 
static double _hoc_ghk(void* _vptr) {
 double _r;
    _hoc_setdata(_vptr);
 _r =  ghk (  *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 return(_r);
}
 
static int _ode_count(int _type){ return 3;}
 
static void _ode_spec(NrnThread* _nt, _Memb_list* _ml, int _type) {
   Datum* _thread;
   Node* _nd; double _v; int _iml, _cntml;
  _cntml = _ml->_nodecount;
  _thread = _ml->_thread;
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
    _nd = _ml->_nodelist[_iml];
    v = NODEV(_nd);
  cai = _ion_cai;
  cao = _ion_cao;
     _ode_spec2 ();
  }}
 
static void _ode_map(int _ieq, double** _pv, double** _pvdot, double* _pp, Datum* _ppd, double* _atol, int _type) { 
 	int _i; _p = _pp; _ppvar = _ppd;
	_cvode_ieq = _ieq;
	for (_i=0; _i < 3; ++_i) {
		_pv[_i] = _pp + _slist2[_i];  _pvdot[_i] = _pp + _dlist2[_i];
		_cvode_abstol(_atollist, _atol, _i);
	}
 }
 
static void _ode_matsol_instance2(_threadargsproto_) {
 _ode_matsol2 ();
 }
 
static void _ode_matsol(NrnThread* _nt, _Memb_list* _ml, int _type) {
   Datum* _thread;
   Node* _nd; double _v; int _iml, _cntml;
  _cntml = _ml->_nodecount;
  _thread = _ml->_thread;
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
    _nd = _ml->_nodelist[_iml];
    v = NODEV(_nd);
  cai = _ion_cai;
  cao = _ion_cao;
 _ode_matsol_instance2(_threadargs_);
 }}
 extern void nrn_update_ion_pointer(Symbol*, Datum*, int, int);
 static void _update_ion_pointer(Datum* _ppvar) {
   nrn_update_ion_pointer(_ca_sym, _ppvar, 2, 1);
   nrn_update_ion_pointer(_ca_sym, _ppvar, 3, 2);
   nrn_update_ion_pointer(_ca_sym, _ppvar, 4, 3);
   nrn_update_ion_pointer(_ca_sym, _ppvar, 5, 4);
 }

static void initmodel() {
  int _i; double _save;_ninits++;
 _save = t;
 t = 0.0;
{
  A = A0;
  B = B0;
  C = C0;
  tmax = tmax0;
 {
   taurise = pow( q10 , ( - ( celsius - T_exp ) / 10.0 ) ) * taurise_exp ;
   taufast = pow( q10 , ( - ( celsius - T_exp ) / 10.0 ) ) * taufast_exp ;
   tauslow = pow( q10 , ( - ( celsius - T_exp ) / 10.0 ) ) * tauslow_exp ;
   aslow = 1.0 - afast ;
   tmax = log ( taufast / taurise ) / ( 1.0 / taurise - 1.0 / taufast ) ;
   error = newton(1,_slist1, _p, peak, _dlist1);
 if(error){fprintf(stderr,"at line 23 in file triexpsyn.inc:\n    SOLVE peak\n"); nrn_complain(_p); abort_run(error);}
 normfac = - exp ( - tmax / taurise ) + afast * exp ( - tmax / taufast ) + aslow * exp ( - tmax / tauslow ) ;
   total = 0.0 ;
   A = 0.0 ;
   B = 0.0 ;
   C = 0.0 ;
   }
  _sav_indep = t; t = _save;

}
}

static void nrn_init(NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; double _v; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
 _tsav = -1e20;
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 v = _v;
  cai = _ion_cai;
  cao = _ion_cao;
 initmodel();
 }}

static double _nrn_current(double _v){double _current=0.;v=_v;{ {
   g = ( B + C - A ) ;
   i = g * ( 1.0 - fracca ) * ( v - e ) ;
   ica = g * fracca * ghkg ( _threadargscomma_ v , cai , cao , z ) ;
   }
 _current += i;
 _current += ica;

} return _current;
}

static void nrn_cur(NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
  cai = _ion_cai;
  cao = _ion_cao;
 _g = _nrn_current(_v + .001);
 	{ double _dica;
  _dica = ica;
 _rhs = _nrn_current(_v);
  _ion_dicadv += (_dica - ica)/.001 * 1.e2/ (_nd_area);
 	}
 _g = (_g - _rhs)/.001;
  _ion_ica += ica * 1.e2/ (_nd_area);
 _g *=  1.e2/(_nd_area);
 _rhs *= 1.e2/(_nd_area);
#if CACHEVEC
  if (use_cachevec) {
	VEC_RHS(_ni[_iml]) -= _rhs;
  }else
#endif
  {
	NODERHS(_nd) -= _rhs;
  }
 
}}

static void nrn_jacob(NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml];
#if CACHEVEC
  if (use_cachevec) {
	VEC_D(_ni[_iml]) += _g;
  }else
#endif
  {
     _nd = _ml->_nodelist[_iml];
	NODED(_nd) += _g;
  }
 
}}

static void nrn_state(NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; double _v = 0.0; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
 _nd = _ml->_nodelist[_iml];
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 v=_v;
{
  cai = _ion_cai;
  cao = _ion_cao;
 { error =  state();
 if(error){fprintf(stderr,"at line 56 in file ampa_forti.mod:\n	  SOLVE state METHOD cnexp\n"); nrn_complain(_p); abort_run(error);}
 } }}

}

static void terminal(){}

static void _initlists() {
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = tmax_columnindex;
 _slist2[0] = A_columnindex;  _dlist2[0] = DA_columnindex;
 _slist2[1] = B_columnindex;  _dlist2[1] = DB_columnindex;
 _slist2[2] = C_columnindex;  _dlist2[2] = DC_columnindex;
_first = 0;
}

#if NMODL_TEXT
static const char* nmodl_filename = "ampa_forti.mod";
static const char* nmodl_file_text = 
  "COMMENT\n"
  "\n"
  "Forti etal's (1997) AMPA synapse with two decay time constants based on the \n"
  "exp2syn.mod mechanism from the NEURON distribution\n"
  "\n"
  "The value of 0.002 for the fraction of Ca flowing at low membrane\n"
  "potentials is derived from the measurements of Spruston &al (1995)\n"
  "\n"
  "Forti & al. 1997 \"Loose-patch recordings of single quanta at\n"
  "  individual hippocampal synapses\" Nature 388, 874-878\n"
  "\n"
  "Spruston & al. 1995 \"Dendritic glutamate receptor channels in rat\n"
  "  hippocampal CA3 and CA1 neurons\" J. Physiol. 482, 325-352\n"
  "\n"
  "ENDCOMMENT\n"
  "\n"
  "NEURON {\n"
  "	  POINT_PROCESS AmpaSyn\n"
  "	  RANGE  e, g, i, ica\n"
  "	  NONSPECIFIC_CURRENT i\n"
  "    USEION ca READ cai,cao WRITE ica\n"
  "    GLOBAL total, taurise, taufast, tauslow, taurise_exp, taufast_exp, tauslow_exp, afast, aslow, normfac, T_exp, fracca\n"
  "}\n"
  "\n"
  ":::INCLUDE \"units.inc\"\n"
  "UNITS {\n"
  "    (molar) = (/liter)\n"
  "    (mA) = (milliamp)\n"
  "    (nA) = (nanoamp)\n"
  "    (mV) = (millivolt)\n"
  "    (mM) = (millimolar)\n"
  "    (S) = (siemens)\n"
  "    (uS) = (microsiemens)\n"
  "    FARADAY = (faraday) (coulomb)\n"
  "    R = (k-mole) (joule/degC)\n"
  "}\n"
  ":::end INCLUDE units.inc\n"
  "\n"
  "PARAMETER {\n"
  "	  taurise_exp =.2 (ms) <1e-9,1e9>       : rise\n"
  "	  taufast_exp = 0.61 (ms) <1e-9,1e9>    : fast decay\n"
  "	  tauslow_exp = 2.55 (ms) <1e-9,1e9>    : slow decay\n"
  "    afast = 0.75 <0,1>\n"
  "	  e=0	(mV)\n"
  "    fracca= 0.002        : fraction of current that is ca ions\n"
  "    z = 2\n"
  "    celsius = 22	(degC)\n"
  "    T_exp = 22    (degC)\n"
  "    q10 = 3\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "    v       (mV)\n"
  "    i       (nA)\n"
  "    ica	    (nA) 	\n"
  "    g       (uS)\n"
  "    aslow \n"
  "    total   (uS)\n"
  "    cai     (mM)\n"
  "    cao     (mM)\n"
  "    taurise (ms)\n"
  "    taufast (ms)\n"
  "    tauslow (ms)\n"
  "    normfac \n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	  SOLVE state METHOD cnexp\n"
  "    g = (B + C - A) \n"
  "    i =   g * (1-fracca) * (v - e)\n"
  "    ica = g * fracca     * ghkg(v,cai,cao,z)\n"
  "}\n"
  "\n"
  ":::INCLUDE \"triexpsyn.inc\"\n"
  "STATE {\n"
  "    A (uS)        : rising component\n"
  "    B (uS)        : fast decyaing component\n"
  "    C (uS)        : slow decyaing component\n"
  "    tmax (ms)     : point at which function is maximal\n"
  "}\n"
  "\n"
  "NONLINEAR peak {\n"
  "    ~ 1/taurise*exp(-tmax/taurise)-afast/taufast*exp(-tmax/taufast)-aslow/tauslow*exp(-tmax/tauslow) = 0\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "    taurise = q10^(-(celsius-T_exp)/10(degC))*taurise_exp\n"
  "    taufast = q10^(-(celsius-T_exp)/10(degC))*taufast_exp\n"
  "    tauslow = q10^(-(celsius-T_exp)/10(degC))*tauslow_exp\n"
  "    aslow = 1 - afast\n"
  "    : Estimate time of peak\n"
  "    tmax = log(taufast/taurise)/(1/taurise-1/taufast) \n"
  "    : normfac = -exp(-tmax/taurise)+afast*exp(-tmax/taufast)+aslow*exp(-tmax/tauslow)\n"
  "    : printf(\"tmax: %g, normfac: %g\\n\", tmax, normfac)\n"
  "    : Find time of peak and normfac numerically\n"
  "    : (seem to be quite similar to estimate, so maybe this is a waste of time!)\n"
  "    SOLVE peak\n"
  "    normfac = -exp(-tmax/taurise)+afast*exp(-tmax/taufast)+aslow*exp(-tmax/tauslow)\n"
  "    : printf(\"tauslow: %g, tmax: %g, normfac: %g\\n\", tauslow, tmax, normfac)\n"
  "    total = 0\n"
  "    A = 0\n"
  "    B = 0\n"
  "    C = 0\n"
  "}\n"
  "\n"
  "DERIVATIVE state {\n"
  "    A' = -A/taurise\n"
  "    B' = -B/taufast\n"
  "    C' = -C/tauslow\n"
  "}\n"
  "\n"
  "NET_RECEIVE(weight (uS)) {\n"
  "    LOCAL normweight\n"
  "    normweight = weight/normfac\n"
  "    state_discontinuity(A, A + normweight)\n"
  "    state_discontinuity(B, B + normweight*afast)\n"
  "    state_discontinuity(C, C + normweight*aslow)\n"
  "    total = total+normweight\n"
  "}\n"
  ":::end INCLUDE triexpsyn.inc\n"
  "\n"
  ":::INCLUDE \"ghk.inc\"\n"
  "COMMENT\n"
  "    GHK function that returns effective driving force\n"
  "    Slope at low voltages is 1\n"
  "    z needs to be set as a PARAMETER\n"
  "ENDCOMMENT\n"
  "\n"
  "FUNCTION ghkg(v(mV), ci(mM), co(mM), z) (mV) {\n"
  "    LOCAL xi, f, exi, fxi\n"
  "    f = R*(celsius+273.15)/(z*(1e-3)*FARADAY)\n"
  "    xi = v/f\n"
  "    exi = exp(xi)\n"
  "    if (fabs(xi) < 1e-4) {\n"
  "        fxi = 1 - xi/2\n"
  "    }else{\n"
  "        fxi = xi/(exi - 1)\n"
  "    }\n"
  "    ghkg = f*((ci/co)*exi - 1)*fxi\n"
  "}\n"
  "\n"
  "FUNCTION ghk(v(mV), ci(mM), co(mM), z) (.001 coul/cm3) {\n"
  "    LOCAL xi, f, exi, fxi\n"
  "    f = R*(celsius+273.15)/(z*(1e-3)*FARADAY)\n"
  "    xi = v/f\n"
  "    exi = exp(xi)\n"
  "    if (fabs(xi) < 1e-4) {\n"
  "        fxi = 1 - xi/2\n"
  "    }else{\n"
  "        fxi = xi/(exi - 1)\n"
  "    }\n"
  "    ghk = (.001)*z*FARADAY*(ci*exi - co)*fxi\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  ":::end INCLUDE ghk.inc\n"
  "\n"
  "\n"
  ;
#endif
