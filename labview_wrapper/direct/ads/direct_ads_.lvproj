<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="17008000">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">false</Property>
	<Property Name="NI.Project.Description" Type="Str">Interface to the SorpPropLib that contains adsorption functions.</Property>
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
			<Item Name="direct_ads_dp_dT_wT.vi" Type="VI" URL="../direct_ads_dp_dT_wT.vi"/>
			<Item Name="direct_ads_dp_dw_wT.vi" Type="VI" URL="../direct_ads_dp_dw_wT.vi"/>
			<Item Name="direct_ads_dw_dp_pT.vi" Type="VI" URL="../direct_ads_dw_dp_pT.vi"/>
			<Item Name="direct_ads_dw_dT_pT.vi" Type="VI" URL="../direct_ads_dw_dT_pT.vi"/>
			<Item Name="direct_ads_p_wT.vi" Type="VI" URL="../direct_ads_p_wT.vi"/>
			<Item Name="direct_ads_piStar_pyxgTM.vi" Type="VI" URL="../direct_ads_piStar_pyxgTM.vi"/>
			<Item Name="direct_ads_T_pw.vi" Type="VI" URL="../direct_ads_T_pw.vi"/>
			<Item Name="direct_ads_w_pT.vi" Type="VI" URL="../direct_ads_w_pT.vi"/>
		</Item>
		<Item Name="Surface" Type="Folder">
			<Item Name="direct_ads_sur_dp_dT_wT.vi" Type="VI" URL="../direct_ads_sur_dp_dT_wT.vi"/>
			<Item Name="direct_ads_sur_dp_dw_wT.vi" Type="VI" URL="../direct_ads_sur_dp_dw_wT.vi"/>
			<Item Name="direct_ads_sur_dw_dp_pT.vi" Type="VI" URL="../direct_ads_sur_dw_dp_pT.vi"/>
			<Item Name="direct_ads_sur_dw_dT_pT.vi" Type="VI" URL="../direct_ads_sur_dw_dT_pT.vi"/>
			<Item Name="direct_ads_sur_p_wT.vi" Type="VI" URL="../direct_ads_sur_p_wT.vi"/>
			<Item Name="direct_ads_sur_piStar_pyxgTM.vi" Type="VI" URL="../direct_ads_sur_piStar_pyxgTM.vi"/>
			<Item Name="direct_ads_sur_T_pw.vi" Type="VI" URL="../direct_ads_sur_T_pw.vi"/>
			<Item Name="direct_ads_sur_w_pT.vi" Type="VI" URL="../direct_ads_sur_w_pT.vi"/>
		</Item>
		<Item Name="Surface_vapor" Type="Folder">
			<Item Name="direct_ads_sur_dp_dT_wTpsat.vi" Type="VI" URL="../direct_ads_sur_dp_dT_wTpsat.vi"/>
			<Item Name="direct_ads_sur_dp_dw_wTpsat.vi" Type="VI" URL="../direct_ads_sur_dp_dw_wTpsat.vi"/>
			<Item Name="direct_ads_sur_dw_dp_pTsat.vi" Type="VI" URL="../direct_ads_sur_dw_dp_pTsat.vi"/>
			<Item Name="direct_ads_sur_dw_dT_pTpsat.vi" Type="VI" URL="../direct_ads_sur_dw_dT_pTpsat.vi"/>
			<Item Name="direct_ads_sur_p_wTpsat.vi" Type="VI" URL="../direct_ads_sur_p_wTpsat.vi"/>
			<Item Name="direct_ads_sur_piStar_pyxgTpsatM.vi" Type="VI" URL="../direct_ads_sur_piStar_pyxgTpsatM.vi"/>
			<Item Name="direct_ads_sur_T_pwpsat.vi" Type="VI" URL="../direct_ads_sur_T_pwpsat.vi"/>
			<Item Name="direct_ads_sur_w_pTpsat.vi" Type="VI" URL="../direct_ads_sur_w_pTpsat.vi"/>
		</Item>
		<Item Name="Volume" Type="Folder">
			<Item Name="direct_ads_vol_A_WRho.vi" Type="VI" URL="../direct_ads_vol_A_WRho.vi"/>
			<Item Name="direct_ads_vol_dA_dW_WRho.vi" Type="VI" URL="../direct_ads_vol_dA_dW_WRho.vi"/>
			<Item Name="direct_ads_vol_dp_dT_wTpsatRho.vi" Type="VI" URL="../direct_ads_vol_dp_dT_wTpsatRho.vi"/>
			<Item Name="direct_ads_vol_dp_dw_wTpsatRho.vi" Type="VI" URL="../direct_ads_vol_dp_dw_wTpsatRho.vi"/>
			<Item Name="direct_ads_vol_dW_dA_ARho.vi" Type="VI" URL="../direct_ads_vol_dW_dA_ARho.vi"/>
			<Item Name="direct_ads_vol_dw_dp_pTpsatRho.vi" Type="VI" URL="../direct_ads_vol_dw_dp_pTpsatRho.vi"/>
			<Item Name="direct_ads_vol_dw_dT_pTpsatRho.vi" Type="VI" URL="../direct_ads_vol_dw_dT_pTpsatRho.vi"/>
			<Item Name="direct_ads_vol_p_wTpsatRho.vi" Type="VI" URL="../direct_ads_vol_p_wTpsatRho.vi"/>
			<Item Name="direct_ads_vol_piStar_pyxgTpsatRhoM.vi" Type="VI" URL="../direct_ads_vol_piStar_pyxgTpsatRhoM.vi"/>
			<Item Name="direct_ads_vol_T_pwpsatRho.vi" Type="VI" URL="../direct_ads_vol_T_pwpsatRho.vi"/>
			<Item Name="direct_ads_vol_W_ARho.vi" Type="VI" URL="../direct_ads_vol_W_ARho.vi"/>
			<Item Name="direct_ads_vol_w_pTpsatRho.vi" Type="VI" URL="../direct_ads_vol_w_pTpsatRho.vi"/>
		</Item>
		<Item Name="Volume_vapor" Type="Folder"/>
		<Item Name="Abhängigkeiten" Type="Dependencies">
			<Item Name="libsorpPropLib.dll" Type="Document" URL="../../../data/win32bit/libsorpPropLib.dll"/>
		</Item>
		<Item Name="Build-Spezifikationen" Type="Build"/>
	</Item>
</Project>
