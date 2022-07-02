/*
Configuration of keybindings visible in the keybindings menu.
*/

//------------------------------------------------------------------------------------------------


[BaseContainerProps(configRoot: true)]
class SF_ModMenuConfig : Managed
{			
	[Attribute("", UIWidgets.Object, "", "")]
	ref array<ref SF_ModList> modListItems;
};


[BaseContainerProps("", "Mod Settings Framework"), BaseContainerCustomTitleField("name")]
class SF_ModList
{
	[Attribute("", UIWidgets.LocaleEditBox, "Mod Name")]
	LocalizedString displayName;
	
	[Attribute("", UIWidgets.Object, "", "")]
	ref array<ref SF_ModSettingEntry> modSettingEntries;
};

[BaseContainerProps("", "Mod Setting"), BaseContainerCustomTitleField("settingName")]
class SF_ModSettingEntry
{
	[Attribute("", UIWidgets.EditBox, "Setting name")]
	string settingName;
	
	[Attribute("", UIWidgets.LocaleEditBox, "Visible name of the setting, long texts are not supported, name shouldn't be over 15 characters")]
	LocalizedString displayName;
	
	[Attribute("0", UIWidgets.ComboBox, "Type", enums: ParamEnumArray.FromEnum(ESettingTypes))]
	ESettingTypes settingType;
	
	[Attribute("", UIWidgets.EditBox, "Default Value")]
	string settingDefaultValue;
	
	[Attribute("", UIWidgets.Object, "", "")]
	ref array<ref SF_ModSettingOptions> modSettingOptions;
};

[BaseContainerProps("", "Options"), BaseContainerCustomTitleField("name")]
class SF_ModSettingOptions
{
	[Attribute("", UIWidgets.EditBox, "Key")]
	string optionKey;
	
	[Attribute("", UIWidgets.LocaleEditBox, "Value")]
	LocalizedString optionValue;
};