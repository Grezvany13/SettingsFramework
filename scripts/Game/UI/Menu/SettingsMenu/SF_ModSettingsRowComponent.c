class SF_ModSettingsRowComponent : ScriptedWidgetComponent
{
	protected static Widget s_RootWidget;
	protected static SF_ModSettingsSubMenu s_ModSettingsMenuComponent;
	protected Widget m_ParentWidget;
	
	protected string m_sSettingName;
	protected string m_sSettingDisplayName;
	protected string m_sSettingType;
	

	protected static const string COMBOBOX_LAYOUT_PATH = "{4B5AE6E64037FFB4}UI/layouts/WidgetLibrary/ComboBox/WLib_ComboBox.layout";
	protected static const string SPINBOX_LAYOUT_PATH = "{C9DF0E6590F6C388}UI/layouts/WidgetLibrary/SpinBox/WLib_SpinBox.layout";
	protected static const string EDITBOX_LAYOUT_PATH = "{0022F0B45ADBC5AC}UI/layouts/WidgetLibrary/EditBox/WLib_EditBox.layout";
	
	protected Widget typeWidget;

	//------------------------------------------------------------------------------------------------
	void Create(Widget parentWidget, string settingDisplayName, string settingName, SF_ModSettingsSubMenu menuComponent, string type, Widget rootWidget)
	{
		Print("SF_ModSettingsRowComponent::Create");
		
		//set up globals
		s_RootWidget = rootWidget;
		m_sSettingType = type;
		m_sSettingName = settingName;
		s_ModSettingsMenuComponent = menuComponent;
		m_sSettingDisplayName = settingDisplayName;
		m_ParentWidget = parentWidget;

		RichTextWidget textBox = RichTextWidget.Cast(m_ParentWidget.FindAnyWidget("Name"));
		textBox.SetText(settingDisplayName);
		textBox.ElideText(1, 1.0, "...");

		Print("Type = " + type);
		
		switch(type) {
			case "0":
				typeWidget = GetGame().GetWorkspace().CreateWidgets(COMBOBOX_LAYOUT_PATH, m_ParentWidget.FindAnyWidget("HorizontalLayout1"));
			break;
			case "1":
				typeWidget = GetGame().GetWorkspace().CreateWidgets(SPINBOX_LAYOUT_PATH, m_ParentWidget.FindAnyWidget("HorizontalLayout1"));
			break;
			default:
			case "2":
				typeWidget = GetGame().GetWorkspace().CreateWidgets(EDITBOX_LAYOUT_PATH, m_ParentWidget.FindAnyWidget("HorizontalLayout1"));
			break;
		}
	}
}