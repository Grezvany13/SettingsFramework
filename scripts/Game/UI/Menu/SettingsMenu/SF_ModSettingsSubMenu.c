enum ESettingTypes {
	Seperator,
	ComboBox,
	SpinBox,
	EditBox
};

//------------------------------------------------------------------------------------------------
class SF_ModSettingsSubMenu: SCR_SettingsSubMenuBase
{
	protected ref array<ref SF_SettingsBindingMod> m_aModSettingsBindings = {};
	
	protected ref SF_ModMenuConfig m_settingsConfig;
	
	// Widgets
	protected VerticalLayoutWidget m_wActionsLayout;

	// Resources
	protected static const string COMBOBOX_LAYOUT_PATH = "{4B5AE6E64037FFB4}UI/layouts/WidgetLibrary/ComboBox/WLib_ComboBox.layout";
	protected static const string SPINBOX_LAYOUT_PATH = "{C9DF0E6590F6C388}UI/layouts/WidgetLibrary/SpinBox/WLib_SpinBox.layout";
	protected static const string EDITBOX_LAYOUT_PATH = "{0022F0B45ADBC5AC}UI/layouts/WidgetLibrary/EditBox/WLib_EditBox.layout";
	
	protected static const string SEPARATOR_LAYOUT_PATH = "{81BE767437998C4C}UI/layouts/Menus/SettingsMenu/ModSettingsMenu/ModSettingSeparator.layout";
	
	protected static const string SETTINGS_CONFIG = "{6ACCE116C70654D3}Configs/Mods/SettingsFramework.conf";
	
	//------------------------------------------------------------------------------------------------
	override void OnMenuOpen(SCR_SuperMenuBase parentMenu)
	{
		
		Print("SF_ModSettingsSubMenu::OnMenuOpen");
		
		
		super.OnMenuOpen(parentMenu);
		m_wActionsLayout = VerticalLayoutWidget.Cast(GetRootWidget().FindAnyWidget("ActionRowsContent"));
		if (!m_wActionsLayout)
			return;
		
		Resource holder = BaseContainerTools.LoadContainer(SETTINGS_CONFIG);
		BaseContainer container = holder.GetResource().ToBaseContainer();
		m_settingsConfig = SF_ModMenuConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(container));

		InsertModsToComboBox();
		ListSettingsFromCurrentMod();
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void LoadSettings(bool forceLoadSettings = false, bool addEntryChangedEventHandler = true)
	{
		Print("SF_ModSettingsSubMenu::LoadSettings");
		
		if (!m_wScroll)
			return;

		m_bLoadingSettings = true;
		foreach (SF_SettingsBindingMod bind : m_aModSettingsBindings)
		{
			bind.LoadEntry(m_wScroll, forceLoadSettings, addEntryChangedEventHandler);
			if (addEntryChangedEventHandler)
				bind.GetEntryChangedInvoker().Insert(OnMenuItemChanged);
		}
		m_bLoadingSettings = false;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void InsertModsToComboBox()
	{
		Print("SF_ModSettingsSubMenu::InsertModsToComboBox");
		
		Widget spinBox = GetRootWidget().FindAnyWidget("ModListBox");
		if (!spinBox)
			return;

		SCR_SpinBoxComponent spinBoxComponent = SCR_SpinBoxComponent.Cast(spinBox.GetHandler(0));
		if (!spinBoxComponent)
			return;

		spinBoxComponent.ClearAll();
		spinBoxComponent.m_OnChanged.Insert(ListSettingsFromCurrentMod);

		foreach (SF_ModList modListItem : m_settingsConfig.modListItems)
			spinBoxComponent.AddItem(modListItem.displayName);

		spinBoxComponent.SetCurrentItem(0);
	}
	
	//------------------------------------------------------------------------------------------------
	void ListSettingsFromCurrentMod()
	{
		Print("SF_ModSettingsSubMenu::ListSettingsFromCurrentMod");
		
		Widget spinBox = GetRootWidget().FindAnyWidget("ModListBox");
		bool firstSeparator = true;
		if (!spinBox)
			return;

		SCR_SpinBoxComponent spinBoxComponent = SCR_SpinBoxComponent.Cast(spinBox.GetHandler(0));
		if (!spinBoxComponent)
			return;

		SF_ModList modListItem = m_settingsConfig.modListItems.Get(spinBoxComponent.GetCurrentIndex());
		if (!modListItem)
			return;

		Widget rowToDelete = m_wActionsLayout.GetChildren();
		while (rowToDelete)
		{
			m_wActionsLayout.RemoveChild(rowToDelete);
			rowToDelete = m_wActionsLayout.GetChildren();
		}

		string displayName;
		
		Widget actionRowWidget;
		TextWidget actionRowWidgetText;
		
		Widget separator;
		TextWidget separatorText;
		
		foreach (SF_ModSettingEntry entry : modListItem.modSettingEntries)
		{
			displayName = entry.displayName;
			if (displayName.Length() == 0)
				displayName = "<#AR-Settings_Keybind_MissingName>" + entry.settingName;

			switch(entry.settingType)
			{
				// Seperator
				case 0:
					separator = GetGame().GetWorkspace().CreateWidgets(SEPARATOR_LAYOUT_PATH, m_wActionsLayout);
					if (!separator)
						continue;
					separatorText = TextWidget.Cast(separator.FindAnyWidget("ActionCategoryName"));
					if (!separatorText)
						continue;
					separatorText.SetVisible(!entry.displayName.IsEmpty());
					separatorText.SetText(entry.displayName);
				break;
				// ComboBox
				case 1:
					actionRowWidget = GetGame().GetWorkspace().CreateWidgets(COMBOBOX_LAYOUT_PATH, m_wActionsLayout);
					if (!actionRowWidget)
						continue;
					actionRowWidgetText = TextWidget.Cast(actionRowWidget.FindAnyWidget("Text"));
					if(!actionRowWidgetText)
						continue;
					actionRowWidgetText.SetVisible(!entry.displayName.IsEmpty());
					actionRowWidgetText.SetText(entry.displayName);
				
					SCR_ComboBoxComponent actionComboBoxComponent = SCR_ComboBoxComponent.Cast(actionRowWidget.GetHandler(0));
					if (!actionComboBoxComponent)
						return;
					
					actionComboBoxComponent.ClearAll();
					actionComboBoxComponent.m_OnChanged.Insert(OnMenuItemUpdate);
					
					foreach (SF_ModSettingOptions option : entry.modSettingOptions)
					{
						actionComboBoxComponent.AddItem(option.optionValue);
					}
					actionComboBoxComponent.SetCurrentItem(0);
				break;
				// SpinBox
				case 2:
					actionRowWidget = GetGame().GetWorkspace().CreateWidgets(SPINBOX_LAYOUT_PATH, m_wActionsLayout);
					if (!actionRowWidget)
						continue;
					actionRowWidgetText = TextWidget.Cast(actionRowWidget.FindAnyWidget("Text"));
					if(!actionRowWidgetText)
						continue;
					actionRowWidgetText.SetVisible(!entry.displayName.IsEmpty());
					actionRowWidgetText.SetText(entry.displayName);
				
					SCR_SpinBoxComponent actionSpinBoxComponent = SCR_SpinBoxComponent.Cast(actionRowWidget.GetHandler(0));
					if (!actionSpinBoxComponent)
						return;
					
					actionSpinBoxComponent.ClearAll();
					actionSpinBoxComponent.m_OnChanged.Insert(OnMenuItemUpdate);
					
					foreach (SF_ModSettingOptions option : entry.modSettingOptions)
					{
						actionSpinBoxComponent.AddItem(option.optionValue);
					}
					actionSpinBoxComponent.SetCurrentItem(0);
				
				break;
				// EditBox
				default:
				case 3:
					actionRowWidget = GetGame().GetWorkspace().CreateWidgets(EDITBOX_LAYOUT_PATH, m_wActionsLayout);
					if (!actionRowWidget)
						continue;
					actionRowWidgetText = TextWidget.Cast(actionRowWidget.FindAnyWidget("Text"));
					if(!actionRowWidgetText)
						continue;
					actionRowWidgetText.SetVisible(!entry.displayName.IsEmpty());
					actionRowWidgetText.SetText(entry.displayName);
				
					SCR_EditBoxComponent actionEditBoxComponent = SCR_EditBoxComponent.Cast(actionRowWidget.GetHandler(0));
					if (!actionEditBoxComponent)
						return;
					
					//actionEditBoxComponent.ClearAll();
					actionEditBoxComponent.m_OnChanged.Insert(OnMenuItemUpdate);
					
					//foreach (SF_ModSettingOptions option : entry.modSettingOptions)
					//{
					//	actionEditBoxComponent.AddItem(option.optionValue);
					//}
					//actionEditBoxComponent.SetCurrentItem(0);
				
					actionEditBoxComponent.SetValue(entry.settingDefaultValue);
				
				break;
			}
		}
	}
	
		//------------------------------------------------------------------------------------------------
	override void OnMenuShow(SCR_SuperMenuBase parentMenu)
	{
		super.OnMenuShow(parentMenu);
		
		Print("SF_ModSettingsSubMenu::OnMenuShow");
		
		ListSettingsFromCurrentMod();
	}

		//------------------------------------------------------------------------------------------------
	protected void OnMenuItemUpdate(Class self)
	{
		Print("SF_ModSettingsSubMenu::OnMenuItemUpdate");
		Print(">> OnMenuItemUpdate");
		Print("---");
		Print(self.ToString());
		Print("---");
		Print(m_settingsConfig.ToString());
		Print("---");
		
		// save config
		Resource holder = BaseContainerTools.CreateContainerFromInstance(m_settingsConfig);
		if (holder)
		{
			BaseContainerTools.SaveContainer(holder.GetResource().ToBaseContainer(), SETTINGS_CONFIG);
		}
	}
};