class SF_SettingsBindingMod : SCR_SettingsBindingBase
{

	//------------------------------------------------------------------------------------------------
	protected override BaseContainer SetBaseContainer()
	{
		UserSettings userSettings;
		userSettings = GetGame().GetGameUserSettings().GetModule("SF_ModSettings");

		if (!userSettings)
			return null;

		if (m_sSubModule.IsEmpty())
			m_Setting = userSettings.GetModule(m_sModule);
		else
		{
		BaseContainer setting = GetGame().GetEngineUserSettings().GetModule(m_sModule);
		if (setting)
				m_Setting = setting.GetObject(m_sSubModule);
		}

		return m_Setting;
	}
};
