# SorpPropLib

A dynamic library for isotherm correlaiton of sorption working pairs. The library is developed to be linked to:

* a standalone batch calculation program with tabular text input and text output [sorpPropBatch](http://github.com/zhiyaoyang/sorppropbatch)

* Sorption system Simulation program (SorpSim)

* Available equations () forms include:
	* Antoine (7)
	* Duhring (2)
	* Toth (21)
	* NRTL ()
	* Custom (2)

* Available pairs (32) data include:
	* water (14)
		* LiBr
		* LiBr:CH3COOK
		* LiBr:CH3CH(OH)COONa
		* LiBr:H2N(CH2)2OH
		* LiBr:HO(CH2)3OH
		* LiBr:LiNO3-LiI-LiCl
		* LiBr:LiI-OH(CH2)3OH
		* liBr:LiNO3-LiI-LiCl
		* LiCl
		* CaCl2
		* Zeolite:5A
		* Zeolite:13X
		* SilicaGel
		* NaOH:KOH-CsOH
	* CO2 (5)
		* Zeolite:5A
		* Zeolite:13X
		* Carbon:ACF(A-20)
		* Carbon:AC-MaxsorbIII
		* SilicaGel
	* propylene (5)
		* Zeolite:4A
		* Zeolite:13X
		* Zeolite:5A-crystal
		* Zeolite:5A-pellets
		* Carbon:MolecularSieve
	* propane (5)
		* Zeolite:4A
		* Zeolite:13X
		* Zeolite:5A-crystal
		* Zeolite:5A-pellets
		* Carbon:MolecularSieve
	* butene (1)
		* Zeolite:13X
	* HFO1234ze (1)
		* Carbon:AC-MaxsorbIII
	* HFC134a (1)
		* Carbon:AC-MaxsorbIII

* Available equation of state of pure components (1) include:
	* water
	* 
	
todo:
* check all available data
* let pure refrigerant class access ref data, and be accessed where EOS and property is needed
* keep white space in comment
* multiple equation for one pair: display all results

Readme last updated 12/4/2017