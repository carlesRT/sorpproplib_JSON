<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="17008000">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">false</Property>
	<Property Name="NI.Project.Description" Type="Str">Interface to the SorpPropLib that contains refrigerant functions.</Property>
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
		<Item Name="General" Type="Folder">
			<Item Name="direct_ref_dp_sat_dT_T.vi" Type="VI" URL="../direct_ref_dp_sat_dT_T.vi"/>
			<Item Name="direct_ref_drho_l_dT_T.vi" Type="VI" URL="../direct_ref_drho_l_dT_T.vi"/>
			<Item Name="direct_ref_p_sat_T.vi" Type="VI" URL="../direct_ref_p_sat_T.vi"/>
			<Item Name="direct_ref_rho_l_T.vi" Type="VI" URL="../direct_ref_rho_l_T.vi"/>
			<Item Name="direct_ref_T_sat_p.vi" Type="VI" URL="../direct_ref_T_sat_p.vi"/>
		</Item>
		<Item Name="Abhängigkeiten" Type="Dependencies">
			<Item Name="libsorpPropLib.dll" Type="Document" URL="../../../data/win32bit/libsorpPropLib.dll"/>
		</Item>
		<Item Name="Build-Spezifikationen" Type="Build"/>
	</Item>
</Project>
