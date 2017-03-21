// (c)2017 Paul Golds, released under MIT License.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProceduralTKDemosTarget : TargetRules
{
	public ProceduralTKDemosTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "ProceduralTKDemos" } );
	}
}
