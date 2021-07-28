<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="17008000">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">false</Property>
	<Property Name="NI.Project.Description" Type="Str">Interface to the SorpPropLib allowing to execute functions based on a working pair struct. That is the proper way of using SorpPropLib as it is faster than direct function calls.</Property>
	<Property Name="SMProvider.SMVersion" Type="Int">201310</Property>
	<Item Name="Mein Computer" Type="My Computer">
		<Property Name="IOScan.Faults" Type="Str"></Property>
		<Property Name="IOScan.NetVarPeriod" Type="UInt">100</Property>
		<Property Name="IOScan.NetWatchdogEnabled" Type="Bool">false</Property>
		<Property Name="IOScan.Period" Type="UInt">10000</Property>
		<Property Name="IOScan.PowerupMode" Type="UInt">0</Property>
		<Property Name="IOScan.Priority" Type="UInt">9</Property>
		<Property Name="IOScan.ReportModeConflict" Type="Bool">true</Property>
		<Property Name="IOScan.StartEngineOnDeploy" Type="Bool">false</Property>
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">Mein Computer/VI-Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">Mein Computer/VI-Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="General" Type="Folder">
			<Item Name="delWorkingPair.vi" Type="VI" URL="../delWorkingPair.vi"/>
			<Item Name="newWorkingPair.vi" Type="VI" URL="../newWorkingPair.vi"/>
		</Item>
		<Item Name="Sub-libraries" Type="Folder">
			<Item Name="struct_abs.lvproj" Type="Document" URL="../abs/struct_abs.lvproj"/>
			<Item Name="struct_ads.lvproj" Type="Document" URL="../ads/struct_ads.lvproj"/>
			<Item Name="struct_ref.lvproj" Type="Document" URL="../ref/struct_ref.lvproj"/>
		</Item>
		<Item Name="Examples" Type="Folder">
			<Item Name="test_struct_ads_surface.vi" Type="VI" URL="../examples/test_struct_ads_surface.vi"/>
			<Item Name="test_struct_ads_surface_vapor.vi" Type="VI" URL="../examples/test_struct_ads_surface_vapor.vi"/>
			<Item Name="test_struct_ads_volume.vi" Type="VI" URL="../examples/test_struct_ads_volume.vi"/>
			<Item Name="test_struct_abs_conventional.vi" Type="VI" URL="../examples/test_struct_abs_conventional.vi"/>
			<Item Name="test_struct_abs_activity.vi" Type="VI" URL="../examples/test_struct_abs_activity.vi"/>
			<Item Name="test_struct_abs_activity_vapor.vi" Type="VI" URL="../examples/test_struct_abs_activity_vapor.vi"/>
			<Item Name="test_struct_abs_mixing.vi" Type="VI" URL="../examples/test_struct_abs_mixing.vi"/>
		</Item>
		<Item Name="Abhängigkeiten" Type="Dependencies">
			<Item Name="libsorpPropLib.dll" Type="Document" URL="../../data/win32bit/libsorpPropLib.dll"/>
			<Item Name="struct_ads_dp_dT_wT.vi" Type="VI" URL="../ads/struct_ads_dp_dT_wT.vi"/>
			<Item Name="struct_ads_dp_dw_wT.vi" Type="VI" URL="../ads/struct_ads_dp_dw_wT.vi"/>
			<Item Name="struct_ads_dw_dp_pT.vi" Type="VI" URL="../ads/struct_ads_dw_dp_pT.vi"/>
			<Item Name="struct_ads_dw_dT_pT.vi" Type="VI" URL="../ads/struct_ads_dw_dT_pT.vi"/>
			<Item Name="struct_ads_p_wT.vi" Type="VI" URL="../ads/struct_ads_p_wT.vi"/>
			<Item Name="struct_ads_sur_dp_dT_wT.vi" Type="VI" URL="../ads/struct_ads_sur_dp_dT_wT.vi"/>
			<Item Name="struct_ads_sur_dp_dT_wTpsat.vi" Type="VI" URL="../ads/struct_ads_sur_dp_dT_wTpsat.vi"/>
			<Item Name="struct_ads_sur_dp_dw_wT.vi" Type="VI" URL="../ads/struct_ads_sur_dp_dw_wT.vi"/>
			<Item Name="struct_ads_sur_dp_dw_wTpsat.vi" Type="VI" URL="../ads/struct_ads_sur_dp_dw_wTpsat.vi"/>
			<Item Name="struct_ads_sur_dw_dp_pT.vi" Type="VI" URL="../ads/struct_ads_sur_dw_dp_pT.vi"/>
			<Item Name="struct_ads_sur_dw_dp_pTsat.vi" Type="VI" URL="../ads/struct_ads_sur_dw_dp_pTsat.vi"/>
			<Item Name="struct_ads_sur_dw_dT_pT.vi" Type="VI" URL="../ads/struct_ads_sur_dw_dT_pT.vi"/>
			<Item Name="struct_ads_sur_dw_dT_pTpsat.vi" Type="VI" URL="../ads/struct_ads_sur_dw_dT_pTpsat.vi"/>
			<Item Name="struct_ads_sur_p_wT.vi" Type="VI" URL="../ads/struct_ads_sur_p_wT.vi"/>
			<Item Name="struct_ads_sur_p_wTpsat.vi" Type="VI" URL="../ads/struct_ads_sur_p_wTpsat.vi"/>
			<Item Name="struct_ads_sur_T_pw.vi" Type="VI" URL="../ads/struct_ads_sur_T_pw.vi"/>
			<Item Name="struct_ads_sur_T_pwpsat.vi" Type="VI" URL="../ads/struct_ads_sur_T_pwpsat.vi"/>
			<Item Name="struct_ads_sur_w_pT.vi" Type="VI" URL="../ads/struct_ads_sur_w_pT.vi"/>
			<Item Name="struct_ads_sur_w_pTpsat.vi" Type="VI" URL="../ads/struct_ads_sur_w_pTpsat.vi"/>
			<Item Name="struct_ads_T_pw.vi" Type="VI" URL="../ads/struct_ads_T_pw.vi"/>
			<Item Name="struct_ads_vol_dp_dT_wTpsatRho.vi" Type="VI" URL="../ads/struct_ads_vol_dp_dT_wTpsatRho.vi"/>
			<Item Name="struct_ads_vol_dp_dw_wTpsatRho.vi" Type="VI" URL="../ads/struct_ads_vol_dp_dw_wTpsatRho.vi"/>
			<Item Name="struct_ads_vol_dw_dp_pTpsatRho.vi" Type="VI" URL="../ads/struct_ads_vol_dw_dp_pTpsatRho.vi"/>
			<Item Name="struct_ads_vol_dw_dT_pTpsatRho.vi" Type="VI" URL="../ads/struct_ads_vol_dw_dT_pTpsatRho.vi"/>
			<Item Name="struct_ads_vol_p_wTpsatRho.vi" Type="VI" URL="../ads/struct_ads_vol_p_wTpsatRho.vi"/>
			<Item Name="struct_ads_vol_T_pwpsatRho.vi" Type="VI" URL="../ads/struct_ads_vol_T_pwpsatRho.vi"/>
			<Item Name="struct_ads_vol_w_pTpsatRho.vi" Type="VI" URL="../ads/struct_ads_vol_w_pTpsatRho.vi"/>
			<Item Name="struct_ads_w_pT.vi" Type="VI" URL="../ads/struct_ads_w_pT.vi"/>
			<Item Name="struct_ref_dp_sat_dT_T.vi" Type="VI" URL="../ref/struct_ref_dp_sat_dT_T.vi"/>
			<Item Name="struct_ref_drho_l_dT_T.vi" Type="VI" URL="../ref/struct_ref_drho_l_dT_T.vi"/>
			<Item Name="struct_ref_p_sat_T.vi" Type="VI" URL="../ref/struct_ref_p_sat_T.vi"/>
			<Item Name="struct_ref_rho_l_T.vi" Type="VI" URL="../ref/struct_ref_rho_l_T.vi"/>
			<Item Name="struct_abs_mix_dp_dx_Tx.vi" Type="VI" URL="../abs/struct_abs_mix_dp_dx_Tx.vi"/>
			<Item Name="struct_abs_mix_dp_dT_Tx.vi" Type="VI" URL="../abs/struct_abs_mix_dp_dT_Tx.vi"/>
			<Item Name="struct_abs_mix_T_px.vi" Type="VI" URL="../abs/struct_abs_mix_T_px.vi"/>
			<Item Name="struct_abs_mix_p_Tx.vi" Type="VI" URL="../abs/struct_abs_mix_p_Tx.vi"/>
			<Item Name="struct_abs_mix_x_pT.vi" Type="VI" URL="../abs/struct_abs_mix_x_pT.vi"/>
			<Item Name="struct_abs_act_g_Txv1v2.vi" Type="VI" URL="../abs/struct_abs_act_g_Txv1v2.vi"/>
			<Item Name="struct_abs_act_p_Txv1v2.vi" Type="VI" URL="../abs/struct_abs_act_p_Txv1v2.vi"/>
			<Item Name="struct_abs_act_x_pTv1v2.vi" Type="VI" URL="../abs/struct_abs_act_x_pTv1v2.vi"/>
			<Item Name="struct_abs_act_x_pTv1v2psat.vi" Type="VI" URL="../abs/struct_abs_act_x_pTv1v2psat.vi"/>
			<Item Name="struct_abs_act_p_Txv1v2psat.vi" Type="VI" URL="../abs/struct_abs_act_p_Txv1v2psat.vi"/>
			<Item Name="struct_abs_con_X_pT.vi" Type="VI" URL="../abs/struct_abs_con_X_pT.vi"/>
			<Item Name="struct_abs_con_p_XT.vi" Type="VI" URL="../abs/struct_abs_con_p_XT.vi"/>
			<Item Name="struct_abs_con_T_pX.vi" Type="VI" URL="../abs/struct_abs_con_T_pX.vi"/>
			<Item Name="struct_abs_con_dp_dT_XT.vi" Type="VI" URL="../abs/struct_abs_con_dp_dT_XT.vi"/>
			<Item Name="struct_abs_con_dp_dX_XT.vi" Type="VI" URL="../abs/struct_abs_con_dp_dX_XT.vi"/>
			<Item Name="struct_abs_con_dX_dp_pT.vi" Type="VI" URL="../abs/struct_abs_con_dX_dp_pT.vi"/>
			<Item Name="struct_abs_con_dX_dT_pT.vi" Type="VI" URL="../abs/struct_abs_con_dX_dT_pT.vi"/>
		</Item>
		<Item Name="Build-Spezifikationen" Type="Build"/>
	</Item>
</Project>
