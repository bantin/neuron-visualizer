/* Created by Language version: 7.7.0 */
/* VECTORIZED */
#define NRN_VECTORIZED 1
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
 
#define nrn_init _nrn_init__nap
#define _nrn_initial _nrn_initial__nap
#define nrn_cur _nrn_cur__nap
#define _nrn_current _nrn_current__nap
#define nrn_jacob _nrn_jacob__nap
#define nrn_state _nrn_state__nap
#define _net_receive _net_receive__nap 
#define _f_states _f_states__nap 
#define states states__nap 
 
#define _threadargscomma_ _p, _ppvar, _thread, _nt,
#define _threadargsprotocomma_ double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt,
#define _threadargs_ _p, _ppvar, _thread, _nt
#define _threadargsproto_ double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *getarg();
 /* Thread safe. No static _p or _ppvar. */
 
#define t _nt->_t
#define dt _nt->_dt
#define K _p[0]
#define K_columnindex 0
#define gnabar _p[1]
#define gnabar_columnindex 1
#define vhalf _p[2]
#define vhalf_columnindex 2
#define g _p[3]
#define g_columnindex 3
#define gmax _p[4]
#define gmax_columnindex 4
#define n _p[5]
#define n_columnindex 5
#define ena _p[6]
#define ena_columnindex 6
#define Dn _p[7]
#define Dn_columnindex 7
#define ina _p[8]
#define ina_columnindex 8
#define v _p[9]
#define v_columnindex 9
#define _g _p[10]
#define _g_columnindex 10
#define _ion_ena	*_ppvar[0]._pval
#define _ion_ina	*_ppvar[1]._pval
#define _ion_dinadv	*_ppvar[2]._pval
 
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
 static Datum* _extcall_thread;
 static Prop* _extcall_prop;
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_states(void);
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

 extern void _nrn_setdata_reg(int, void(*)(Prop*));
 static void _setdata(Prop* _prop) {
 _extcall_prop = _prop;
 }
 static void _hoc_setdata() {
 Prop *_prop, *hoc_getdata_range(int);
 _prop = hoc_getdata_range(_mechtype);
   _setdata(_prop);
 hoc_retpushx(1.);
}
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 "setdata_nap", _hoc_setdata,
 "states_nap", _hoc_states,
 0, 0
};
 
static void _check_states(double*, Datum*, Datum*, NrnThread*); 
static void _check_table_thread(double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt, int _type) {
   _check_states(_p, _ppvar, _thread, _nt);
 }
 /* declare global and static user variables */
#define usetable usetable_nap
 double usetable = 1;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "usetable_nap", 0, 1,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "K_nap", "mV",
 "gnabar_nap", "mho/cm2",
 "vhalf_nap", "mV",
 "g_nap", "mho/cm2",
 "gmax_nap", "mho/cm2",
 0,0
};
 static double n0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "usetable_nap", &usetable_nap,
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
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"nap",
 "K_nap",
 "gnabar_nap",
 "vhalf_nap",
 0,
 "g_nap",
 "gmax_nap",
 0,
 "n_nap",
 0,
 0};
 static Symbol* _na_sym;
 
extern Prop* need_memb(Symbol*);

static void nrn_alloc(Prop* _prop) {
	Prop *prop_ion;
	double *_p; Datum *_ppvar;
 	_p = nrn_prop_data_alloc(_mechtype, 11, _prop);
 	/*initialize range parameters*/
 	K = 1;
 	gnabar = 0;
 	vhalf = -51.9;
 	_prop->param = _p;
 	_prop->param_size = 11;
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 3, _prop);
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_na_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0]._pval = &prop_ion->param[0]; /* ena */
 	_ppvar[1]._pval = &prop_ion->param[3]; /* ina */
 	_ppvar[2]._pval = &prop_ion->param[4]; /* _ion_dinadv */
 
}
 static void _initlists();
 static void _update_ion_pointer(Datum*);
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
extern void _nrn_thread_table_reg(int, void(*)(double*, Datum*, Datum*, NrnThread*, int));
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 void _nap_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("na", -10000.);
 	_na_sym = hoc_lookup("na_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 2, _update_ion_pointer);
     _nrn_thread_table_reg(_mechtype, _check_table_thread);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 11, 3);
  hoc_register_dparam_semantics(_mechtype, 0, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "na_ion");
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 nap nap.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double *_t_n;
static int _reset;
static char *modelname = "Na persistent channel";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int _f_states(_threadargsprotocomma_ double);
static int states(_threadargsprotocomma_ double);
 static void _n_states(_threadargsprotocomma_ double _lv);
 static double _mfac_states, _tmin_states;
  static void _check_states(double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  static double _sav_vhalf;
  static double _sav_K;
  if (!usetable) {return;}
  if (_sav_vhalf != vhalf) { _maktable = 1;}
  if (_sav_K != K) { _maktable = 1;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_states =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_states)/300.; _mfac_states = 1./_dx;
   for (_i=0, _x=_tmin_states; _i < 301; _x += _dx, _i++) {
    _f_states(_p, _ppvar, _thread, _nt, _x);
    _t_n[_i] = n;
   }
   _sav_vhalf = vhalf;
   _sav_K = K;
  }
 }

 static int states(double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt, double _lv) { 
#if 0
_check_states(_p, _ppvar, _thread, _nt);
#endif
 _n_states(_p, _ppvar, _thread, _nt, _lv);
 return 0;
 }

 static void _n_states(double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 _f_states(_p, _ppvar, _thread, _nt, _lv); return; 
}
 _xi = _mfac_states * (_lv - _tmin_states);
 if (isnan(_xi)) {
  n = _xi;
  return;
 }
 if (_xi <= 0.) {
 n = _t_n[0];
 return; }
 if (_xi >= 300.) {
 n = _t_n[300];
 return; }
 _i = (int) _xi;
 _theta = _xi - (double)_i;
 n = _t_n[_i] + _theta*(_t_n[_i+1] - _t_n[_i]);
 }

 
static int  _f_states ( _threadargsprotocomma_ double _lv ) {
   n = 1.0 / ( 1.0 + exp ( ( vhalf - _lv ) / K ) ) ;
    return 0; }
 
static void _hoc_states(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_states(_p, _ppvar, _thread, _nt);
#endif
 _r = 1.;
 states ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 extern void nrn_update_ion_pointer(Symbol*, Datum*, int, int);
 static void _update_ion_pointer(Datum* _ppvar) {
   nrn_update_ion_pointer(_na_sym, _ppvar, 0, 0);
   nrn_update_ion_pointer(_na_sym, _ppvar, 1, 3);
   nrn_update_ion_pointer(_na_sym, _ppvar, 2, 4);
 }

static void initmodel(double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt) {
  int _i; double _save;{
  n = n0;
 {
   gmax = 0.0 ;
   }

}
}

static void nrn_init(NrnThread* _nt, _Memb_list* _ml, int _type){
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; double _v; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];

#if 0
 _check_states(_p, _ppvar, _thread, _nt);
#endif
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
  ena = _ion_ena;
 initmodel(_p, _ppvar, _thread, _nt);
 }
}

static double _nrn_current(double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt, double _v){double _current=0.;v=_v;{ {
   states ( _threadargscomma_ v ) ;
   g = gnabar * n * n * n ;
   ina = g * ( v - ena ) ;
   if ( g > gmax ) {
     gmax = g ;
     }
   }
 _current += ina;

} return _current;
}

static void nrn_cur(NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
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
  ena = _ion_ena;
 _g = _nrn_current(_p, _ppvar, _thread, _nt, _v + .001);
 	{ double _dina;
  _dina = ina;
 _rhs = _nrn_current(_p, _ppvar, _thread, _nt, _v);
  _ion_dinadv += (_dina - ina)/.001 ;
 	}
 _g = (_g - _rhs)/.001;
  _ion_ina += ina ;
#if CACHEVEC
  if (use_cachevec) {
	VEC_RHS(_ni[_iml]) -= _rhs;
  }else
#endif
  {
	NODERHS(_nd) -= _rhs;
  }
 
}
 
}

static void nrn_jacob(NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
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
 
}
 
}

static void nrn_state(NrnThread* _nt, _Memb_list* _ml, int _type) {

}

static void terminal(){}

static void _initlists(){
 double _x; double* _p = &_x;
 int _i; static int _first = 1;
  if (!_first) return;
   _t_n = makevector(301*sizeof(double));
_first = 0;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

#if NMODL_TEXT
static const char* nmodl_filename = "nap.mod";
static const char* nmodl_file_text = 
  "TITLE  Na persistent channel\n"
  ": used in distal oblique dendrites to assist Ca spike initiation  \n"
  ": a typo in the exponential function was pointed out by Michele Migliore and\n"
  ": corrected by Yiota Poirazi on December 4th, 2003\n"
  "NEURON {\n"
  "	  SUFFIX nap\n"
  "	  USEION na READ ena WRITE ina\n"
  "    RANGE  gnabar,vhalf, K, g, gmax\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	  (mA) = (milliamp)\n"
  "	  (mV) = (millivolt)\n"
  "}\n"
  "\n"
  "PARAMETER { : parameters that can be entered when function is called in cell-setup \n"
  "	  v               (mV)\n"
  "    ena = 50        (mV) : Na reversal potential  (reset in cell-setup.hoc)\n"
  "	  K = 1           (mV)              : slope of steady state variable\n"
  "    :	gnabar = 0.001e-2 (mho/cm2) : suggested conductance, 1 percent of the transient Na current\n"
  "	  gnabar = 0      (mho/cm2)          : initialized conductance\n"
  "	  vhalf  = -51.90 (mV)                : half potential\n"
  "}	\n"
  "\n"
  "STATE { n }\n"
  "\n"
  "ASSIGNED {\n"
  "	  ina  (mA/cm2)\n"
  "    g    (mho/cm2)\n"
  "    gmax (mho/cm2)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "    :	SOLVE states not used\n"
  "    gmax = 0\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "    states(v)\n"
  "    g = gnabar*n*n*n\n"
  "	  ina = g*(v-ena)\n"
  "    if (g > gmax) {\n"
  "        gmax = g\n"
  "    }\n"
  "}\n"
  "\n"
  "PROCEDURE states(v (mV)) {     : exact when v held constant; integrates over dt step\n"
  "    :        n = 1 / (1 + (exp(vhalf - v))/K) : steady state value !!!typo in the exponential function!!!\n"
  "    TABLE n DEPEND vhalf, K FROM -150 TO 150 WITH 300     \n"
  "    n = 1 / (1 + exp((vhalf - v)/K)) : steady state value\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  ;
#endif
