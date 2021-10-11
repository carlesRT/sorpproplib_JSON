<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="17008000">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">false</Property>
	<Property Name="NI.Project.Description" Type="Str">Interface to the SorpPropLib that contains absorption functions.</Property>
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
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">Mein Computer/VI-Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">Mein Computer/VI-Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="Activity" Type="Folder">
			<Item Name="struct_abs_act_g_Txv1v2.vi" Type="VI" URL="../struct_abs_act_g_Txv1v2.vi"/>
			<Item Name="struct_abs_act_p_Txv1v2.vi" Type="VI" URL="../struct_abs_act_p_Txv1v2.vi"/>
			<Item Name="struct_abs_act_p_Txv1v2psat.vi" Type="VI" URL="../struct_abs_act_p_Txv1v2psat.vi"/>
			<Item Name="struct_abs_act_x_pTv1v2.vi" Type="VI" URL="../struct_abs_act_x_pTv1v2.vi"/>
			<Item Name="struct_abs_act_x_pTv1v2psat.vi" Type="VI" URL="../struct_abs_act_x_pTv1v2psat.vi"/>
		</Item>
		<Item Name="Conventional" Type="Folder">
			<Item Name="struct_abs_con_dp_dT_XT.vi" Type="VI" URL="../struct_abs_con_dp_dT_XT.vi"/>
			<Item Name="struct_abs_con_dp_dX_XT.vi" Type="VI" URL="../struct_abs_con_dp_dX_XT.vi"/>
			<Item Name="struct_abs_con_dX_dp_pT.vi" Type="VI" URL="../struct_abs_con_dX_dp_pT.vi"/>
			<Item Name="struct_abs_con_dX_dT_pT.vi" Type="VI" URL="../struct_abs_con_dX_dT_pT.vi"/>
			<Item Name="struct_abs_con_p_XT.vi" Type="VI" URL="../struct_abs_con_p_XT.vi"/>
			<Item Name="struct_abs_con_T_pX.vi" Type="VI" URL="../struct_abs_con_T_pX.vi"/>
			<Item Name="struct_abs_con_X_pT.vi" Type="VI" URL="../struct_abs_con_X_pT.vi"/>
		</Item>
		<Item Name="Mixing" Type="Folder">
			<Item Name="struct_abs_mix_dp_dT_Tx.vi" Type="VI" URL="../struct_abs_mix_dp_dT_Tx.vi"/>
			<Item Name="struct_abs_mix_dp_dx_Tx.vi" Type="VI" URL="../struct_abs_mix_dp_dx_Tx.vi"/>
			<Item Name="struct_abs_mix_p_Tx.vi" Type="VI" URL="../struct_abs_mix_p_Tx.vi"/>
			<Item Name="struct_abs_mix_T_px.vi" Type="VI" URL="../struct_abs_mix_T_px.vi"/>
			<Item Name="struct_abs_mix_x_pT.vi" Type="VI" URL="../struct_abs_mix_x_pT.vi"/>
		</Item>
		<Item Name="test_struct_abs_activity_vapor.vi" Type="VI" URL="../../examples/test_struct_abs_activity_vapor.vi"/>
		<Item Name="Abhängigkeiten" Type="Dependencies">
			<Item Name="delWorkingPair.vi" Type="VI" URL="../../delWorkingPair.vi"/>
			<Item Name="libsorpPropLib.dll" Type="Document" URL="../../../data/win32bit/libsorpPropLib.dll"/>
			<Item Name="newWorkingPair.vi" Type="VI" URL="../../newWorkingPair.vi"/>
			<Item Name="struct_ref_p_sat_T.vi" Type="VI" URL="../../ref/struct_ref_p_sat_T.vi"/>
		</Item>
		<Item Name="Build-Spezifikationen" Type="Build"/>
	</Item>
</Project>
