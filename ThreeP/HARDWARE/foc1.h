#ifndef FOC1_H 
	#define FOC1_H 
#endif 

#ifdef _FOC1_C_
   #define _FOC1_EXT 
#else
   #define _FOC1_EXT extern
#endif
	 
#include "main.h"

/**************************************************************************************************
 Definitions & Macros
**************************************************************************************************/
#define CLARKE_DEFAULTS { 0, \
                          0, \
                          0, \
                          0, \
                          0, \
              			  (void (*)(long))clarke_calc }

#define PARK_DEFAULTS {  0, \
                          0, \
                          0, \
                          0, \
                          0, \
              			  (void (*)(long))park_calc }
						  
#define ICLARKE_DEFAULTS { 0, \
                          0, \
                          0, \
                          0, \
                          0, \
              			  (void (*)(long))iclarke_calc }
						  
#define IPARK_DEFAULTS {  0, \
                          0, \
                          0, \
                          0, \
                          0, \
              			  (void (*)(long))ipark_calc }
						  
#define SVGENDQ_DEFAULTS { 0,0,0,0,0,0,0,0,0, \
                       (void (*)(Uint32))svgendq_calc }
						  
/**************************************************************************************************
 TypeDefs & Structure defines (N/A)
**************************************************************************************************/
typedef struct {    float  as;  		/* Input: phase-a stator variable  */
                    float  bs;			/* Input: phase-b stator variable  */
                    float  cs;          /* Input: phase-c stator variable  */
                    float  ds;			/* Output: stationary d-axis stator variable  */
                    float  qs;			/* Output: stationary q-axis stator variable  */
			
		 	 	  void  (*calc)();	/* Pointer to calculation function */ 
				 } CLARKE;	            

typedef CLARKE *CLARKE_handle;

typedef struct {    float  ds;  		/* Input: stationary d-axis stator variable */
                    float  qs;		    /* Input: stationary q-axis stator variable */
                    float  ang;			/* Input: rotating angle (pu) */
                    float  de;			/* Output: rotating d-axis stator variable */
                    float  qe;			/* Output: rotating q-axis stator variable */
		 	 	  void  (*calc)();	/* Pointer to calculation function */ 
				 } PARK;	            

typedef PARK *PARK_handle;

typedef struct {  float  as;  		/* Input: phase-a stator variable  */
				  float  bs;			/* Input: phase-b stator variable  */
				  float  cs;          /* Input: phase-c stator variable  */
				  float  ds;			/* Output: stationary d-axis stator variable  */
				  float  qs;			/* Output: stationary q-axis stator variable  */
			
		 	 	  void  (*calc)();	/* Pointer to calculation function */ 
				 } ICLARKE;	            

typedef ICLARKE *ICLARKE_handle;

typedef struct {  float  ds;  	/* Output: stationary d-axis stator variable */
				  float  qs;		/* Output: stationary q-axis stator variable */
				  float  ang;		/* Input: rotating angle (pu) */
				  float  de;		/* Input: rotating d-axis stator variable */
				  float  qe;		/* Input: rotating q-axis stator variable */
		 	 	  void  (*calc)();	/* Pointer to calculation function */ 
				 } IPARK;	            

typedef IPARK *IPARK_handle;

typedef struct 	{ float  UalfaIn; 		// Input: reference alpha-axis phase voltage
	  	  	  	  float  UbetaIn;			// Input: reference beta-axis phase voltage
				  float  UdcIn;			// Input: DC bus voltage
				  float  Ualfa; 			// Input: reference alpha-axis phase voltage
				  float  Ubeta;			// Input: reference beta-axis phase voltage
				  float  Ta;				// Output: reference phase-a switching function
				  float  Tb;				// Output: reference phase-b switching function
				  float  Tc;				// Output: reference phase-c switching function
				  float  Sector_Flag;
				  void (*calc)();	    // Pointer to calculation function
				} SVGENDQ;
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																				
typedef SVGENDQ *SVGENDQ_handle;

//#if (PRG_MODE == FLASH2RAM)
//  #pragma CODE_SECTION(clarke_calc, "ramfuncs")
//	#pragma CODE_SECTION(park_calc, "ramfuncs")
//	#pragma CODE_SECTION(iclarke_calc, "ramfuncs")
//	#pragma CODE_SECTION(ipark_calc, "ramfuncs")
//	#pragma CODE_SECTION(svgendq_calc, "ramfuncs")
//#endif

extern void clarke_calc(CLARKE_handle);
extern void park_calc(PARK_handle);
extern void iclarke_calc(ICLARKE_handle);
extern void ipark_calc(IPARK_handle);
extern void svgendq_calc(SVGENDQ_handle);

/**************************************************************************************************
 Exported Variables 
**************************************************************************************************/ 

/*#ifdef _FOC1_C_
    _FOC_EXT CLARKE clarke = CLARKE_DEFAULTS;
	_FOC_EXT PARK park = PARK_DEFAULTS;
    _FOC_EXT ICLARKE iclarke = ICLARKE_DEFAULTS;
    _FOC_EXT IPARK ipark = IPARK_DEFAULTS;   
    _FOC_EXT SVGENDQ SV_dq = SVGENDQ_DEFAULTS;

#else*/
extern CLARKE clarke;
extern PARK park;
extern ICLARKE iclarke;
extern IPARK ipark;
extern SVGENDQ SV_dq;
//#endif

/*************************************************************************************************
 End of this File (EOF)!
 Do not put anything after this part!
**************************************************************************************************/
