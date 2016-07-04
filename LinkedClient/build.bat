@echo off

set SRCDIR=..\..\src\
set INCDIR=..\..\include\
set LIBDIR=..\..\lib\

set ProgramDefinitions= -DGLEW_STATIC -DCOMMAND_LINE_COMPILE -D_CRT_SECURE_NO_WARNINGS
set CompilerFlags=-MT -nologo -Gm- -GR- -EHsc -W3 -Zi -MP -Oi -Fo.\obj\ -FeLinkedClient.exe -wd4996
set LinkerFlags= -opt:ref -ignore:4099
set LinkedLibraries= user32.lib gdi32.lib winmm.lib opengl32.lib %LIBDIR%win32\glew32s.lib %LIBDIR%win32\freetype263MT.lib %LIBDIR%win32\sfml-audio.lib ..\..\LinkedClient.res

set CoreFiles= %SRCDIR%Main.cpp %SRCDIR%Core\Audio.cpp %SRCDIR%Core\Game.cpp %SRCDIR%Core\Input.cpp %SRCDIR%Core\LinkedTime.cpp

set GameEntityFiles= %SRCDIR%Entity\Transform.cpp %SRCDIR%Entity\Primitive.cpp %SRCDIR%Entity\Player.cpp %SRCDIR%Entity\Mesh.cpp %SRCDIR%Entity\Item.cpp %SRCDIR%Entity\GameEntity.cpp %SRCDIR%Entity\Equipment.cpp %SRCDIR%Entity\EntityMap.cpp %SRCDIR%Entity\Entity.cpp %SRCDIR%Entity\Cursor.cpp
set GameEntityAIFiles= %SRCDIR%Entity\AI\PlayerAI.cpp %SRCDIR%Entity\AI\MonsterAI.cpp %SRCDIR%Entity\AI\AI.cpp
set GameEntityAttackFiles= %SRCDIR%Entity\Attack\RangeAttack.cpp %SRCDIR%Entity\Attack\Projectile.cpp %SRCDIR%Entity\Attack\MouseAttack.cpp
set GameEntityFactoryFiles= %SRCDIR%Entity\Factory\SkillFactory.cpp %SRCDIR%Entity\Factory\PlayerFactory.cpp %SRCDIR%Entity\Factory\MonsterFactory.cpp %SRCDIR%Entity\Factory\ItemFactory.cpp %SRCDIR%Entity\Factory\GameEntityFactory.cpp %SRCDIR%Entity\Factory\Factory.cpp %SRCDIR%Entity\Factory\EquipmentFactory.cpp
set GameEntityMonsterFiles= %SRCDIR%Entity\Monster\Monster.cpp %SRCDIR%Entity\Monster\TurretMonster.cpp %SRCDIR%Entity\Monster\DistanceMonster.cpp %SRCDIR%Entity\Monster\BomberMonster.cpp %SRCDIR%Entity\Monster\BasicMonster.cpp %SRCDIR%Entity\Monster\Skill\MonsterExplosionSkill.cpp %SRCDIR%Entity\Monster\Skill\MonsterEndureSkill.cpp
set GameEntitySkillFiles= %SRCDIR%Entity\Skill\SwapSkill.cpp %SRCDIR%Entity\Skill\SkillIcon.cpp %SRCDIR%Entity\Skill\Skill.cpp %SRCDIR%Entity\Skill\LinkSkill.cpp %SRCDIR%Entity\Skill\HoshoyoExplosionSkill.cpp %SRCDIR%Entity\Skill\CureBlessingSkill.cpp

set GameGUIFiles= %SRCDIR%GUI\TextRenderer.cpp %SRCDIR%GUI\Menu.cpp %SRCDIR%GUI\HPBar.cpp %SRCDIR%GUI\GUI.cpp %SRCDIR%GUI\Chat.cpp

set GameMapFiles= %SRCDIR%Map\Map.cpp %SRCDIR%Map\Grid.cpp %SRCDIR%Map\MapEntity.cpp %SRCDIR%Map\MapMonster.cpp %SRCDIR%Map\MapTerrain.cpp

set NativeFiles= %SRCDIR%native\WindowInput.cpp %SRCDIR%native\ContextWindow.cpp

set GameNetworkFiles= %SRCDIR%Network\UDPClient.cpp %SRCDIR%Network\PacketController.cpp %SRCDIR%Network\Packet.cpp %SRCDIR%Network\ClientPacket.cpp

set GameResourceLoadFiles= %SRCDIR%ResourceLoad\Texture.cpp %SRCDIR%ResourceLoad\IndexedModel.cpp %SRCDIR%ResourceLoad\ImageLoader.cpp %SRCDIR%ResourceLoad\Configfiles.cpp

set GameShaderFiles= %SRCDIR%Shaders\Light.cpp %SRCDIR%Shaders\FrameBuffer.cpp %SRCDIR%Shaders\Camera.cpp
set GameShaderClassesFiles= %SRCDIR%Shaders\Classes\WindowShader.cpp %SRCDIR%Shaders\Classes\TextShader.cpp %SRCDIR%Shaders\Classes\SkillShader.cpp %SRCDIR%Shaders\Classes\Shader.cpp %SRCDIR%Shaders\Classes\PrimitiveShader.cpp %SRCDIR%Shaders\Classes\MapShader.cpp %SRCDIR%Shaders\Classes\GUIShader.cpp %SRCDIR%Shaders\Classes\FrameShader.cpp %SRCDIR%Shaders\Classes\FontShader.cpp %SRCDIR%Shaders\Classes\CommonShader.cpp

set WindowApiFiles= %SRCDIR%WindowApi\WindowDiv.cpp %SRCDIR%WindowApi\Window.cpp %SRCDIR%WindowApi\Label.cpp %SRCDIR%WindowApi\FontRenderer.cpp %SRCDIR%WindowApi\Button.cpp

set IncludeFolders= -I%INCDIR% -I%INCDIR%Core -I%INCDIR%Entity -I%INCDIR%Entity\Skill -I%INCDIR%Entity\Monster -I%INCDIR%Entity\Monster\Skill -I%INCDIR%Entity\Factory -I%INCDIR%Entity\Attack -I%INCDIR%Entity\AI -I%INCDIR%external -I%INCDIR%GUI -I%INCDIR%Logging -I%INCDIR%Map -I%INCDIR%native -I%INCDIR%Network -I%INCDIR%ResourceLoad -I%INCDIR%Shaders -I%INCDIR%Shaders\Classes -I%INCDIR%WindowApi

set FreeTypeGLFiles= %INCDIR%external\freetypegl\vector.c %INCDIR%external\freetypegl\texture-font.c %INCDIR%external\freetypegl\texture-atlas.c %INCDIR%external\freetypegl\platform.c

set AllCPPFiles=%CoreFiles% %GameEntityFiles% %GameEntityAIFiles% %GameEntityAttackFiles% %GameEntityFactoryFiles% %GameEntityMonsterFiles% %GameEntitySkillFiles% %GameGUIFiles% %GameMapFiles% %NativeFiles% %GameNetworkFiles% %GameResourceLoadFiles% %GameShaderFiles% %GameShaderClassesFiles% %WindowApiFiles% %FreeTypeGLFiles%

IF NOT EXIST bin mkdir bin
pushd bin
IF NOT EXIST cmdbuild mkdir cmdbuild
pushd cmdbuild
IF NOT EXIST obj mkdir obj

cl %ProgramDefinitions% %CompilerFlags% %IncludeFolders% %AllCPPFiles% /link %LinkerFlags% %LinkedLibraries%

popd
popd