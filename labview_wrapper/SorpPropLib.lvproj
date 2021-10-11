<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="17008000">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">false</Property>
	<Property Name="NI.Project.Description" Type="Str">Interface to the SorpPropLib.</Property>
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
		<Item Name="Sub-libraries" Type="Folder">
			<Item Name="direct_call_.lvproj" Type="Document" URL="../direct/direct_call_.lvproj"/>
			<Item Name="struct_call.lvproj" Type="Document" URL="../struct/struct_call.lvproj"/>
		</Item>
		<Item Name="Plots" Type="Folder"/>
		<Item Name="Examples" Type="Folder">
			<Item Name="plot_isoteric_chart.vi" Type="VI" URL="../struct/examples/plot_isoteric_chart.vi"/>
		</Item>
		<Item Name="Example_plot_isoteric_chart.vi" Type="VI" URL="../struct/examples/Example_plot_isoteric_chart.vi"/>
		<Item Name="Abhängigkeiten" Type="Dependencies">
			<Item Name="struct_ref_p_sat_T.vi" Type="VI" URL="../struct/ref/struct_ref_p_sat_T.vi"/>
			<Item Name="libsorpPropLib.dll" Type="Document" URL="../data/win32bit/libsorpPropLib.dll"/>
			<Item Name="newWorkingPair.vi" Type="VI" URL="../struct/newWorkingPair.vi"/>
			<Item Name="struct_ref_T_sat_p.vi" Type="VI" URL="../struct/ref/struct_ref_T_sat_p.vi"/>
			<Item Name="struct_ads_p_wT.vi" Type="VI" URL="../struct/ads/struct_ads_p_wT.vi"/>
			<Item Name="delWorkingPair.vi" Type="VI" URL="../struct/delWorkingPair.vi"/>
			<Item Name="struct_ads_w_pT.vi" Type="VI" URL="../struct/ads/struct_ads_w_pT.vi"/>
		</Item>
		<Item Name="Build-Spezifikationen" Type="Build"/>
	</Item>
</Project>
