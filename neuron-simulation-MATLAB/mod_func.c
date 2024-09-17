#include <stdio.h>
#include "hocdec.h"
#define IMPORT extern __declspec(dllimport)
IMPORT int nrnmpi_myid, nrn_nobanner_;

extern void _ampa_forti_reg();
extern void _cacum_reg();
extern void _cad_reg();
extern void _cagk_reg();
extern void _cal_reg();
extern void _calH_reg();
extern void _car_reg();
extern void _car_mag_reg();
extern void _cat_reg();
extern void _chr_reg();
extern void _kadist_reg();
extern void _kaprox_reg();
extern void _kca_reg();
extern void _kdrca1_reg();
extern void _na3_reg();
extern void _nap_reg();
extern void _somacar_reg();
extern void _vmax_reg();

void modl_reg(){
	//nrn_mswindll_stdio(stdin, stdout, stderr);
    if (!nrn_nobanner_) if (nrnmpi_myid < 1) {
	fprintf(stderr, "Additional mechanisms from files\n");

fprintf(stderr," ampa_forti.mod");
fprintf(stderr," cacum.mod");
fprintf(stderr," cad.mod");
fprintf(stderr," cagk.mod");
fprintf(stderr," cal.mod");
fprintf(stderr," calH.mod");
fprintf(stderr," car.mod");
fprintf(stderr," car_mag.mod");
fprintf(stderr," cat.mod");
fprintf(stderr," chr.mod");
fprintf(stderr," kadist.mod");
fprintf(stderr," kaprox.mod");
fprintf(stderr," kca.mod");
fprintf(stderr," kdrca1.mod");
fprintf(stderr," na3.mod");
fprintf(stderr," nap.mod");
fprintf(stderr," somacar.mod");
fprintf(stderr," vmax.mod");
fprintf(stderr, "\n");
    }
_ampa_forti_reg();
_cacum_reg();
_cad_reg();
_cagk_reg();
_cal_reg();
_calH_reg();
_car_reg();
_car_mag_reg();
_cat_reg();
_chr_reg();
_kadist_reg();
_kaprox_reg();
_kca_reg();
_kdrca1_reg();
_na3_reg();
_nap_reg();
_somacar_reg();
_vmax_reg();
}
