$nick_h = Get-Content "include/server/executor/commands/Nick.hpp" -Raw
$nick_h = $nick_h -replace "Nick\(ServerDao \\\*server = NULL\);", "Nick(ServerDao *server = NULL, Validator *validator = NULL);"
$nick_h = $nick_h -replace "class Nick : public ACommand \{", "class Validator;\n\nclass Nick : public ACommand {\n private:\n  Validator* m_validator;"
[IO.File]::WriteAllText("include/server/executor/commands/Nick.hpp", $nick_h)

$nick_c = Get-Content "src/server/executor/commands/Nick.cpp" -Raw
$nick_c = $nick_c -replace "Nick::Nick\(ServerDao \\\*server\) : ACommand\(server\) \{\}", "Nick::Nick(ServerDao *server, Validator *validator) : ACommand(server), m_validator(validator) {}"
$nick_c = $nick_c -replace "validator::isValidNickname", "m_validator->isValidNickname"
[IO.File]::WriteAllText("src/server/executor/commands/Nick.cpp", $nick_c)

$join_h = Get-Content "include/server/executor/commands/Join.hpp" -Raw
$join_h = $join_h -replace "Join\(ServerDao \\\*server = NULL\);", "Join(ServerDao *server = NULL, Validator *validator = NULL, JoinGatekeeper *joinGatekeeper = NULL);"
$join_h = $join_h -replace "class Join : public ACommand \{", "class Validator;\nclass JoinGatekeeper;\n\nclass Join : public ACommand {\n private:\n  Validator* m_validator;\n  JoinGatekeeper* m_joinGatekeeper;"
[IO.File]::WriteAllText("include/server/executor/commands/Join.hpp", $join_h)

$join_c = Get-Content "src/server/executor/commands/Join.cpp" -Raw
$join_c = $join_c -replace "Join::Join\(ServerDao \\\*server\) : ACommand\(server\) \{\}", "Join::Join(ServerDao *server, Validator *validator, JoinGatekeeper *joinGatekeeper) : ACommand(server), m_validator(validator), m_joinGatekeeper(joinGatekeeper) {}"
$join_c = $join_c -replace "validator::isValidChannelName", "m_validator->isValidChannelName"
$join_c = $join_c -replace "join_gatekeeper::isJoinDenied", "m_joinGatekeeper->isJoinDenied"
[IO.File]::WriteAllText("src/server/executor/commands/Join.cpp", $join_c)

$inv_h = Get-Content "include/server/executor/commands/Invite.hpp" -Raw
$inv_h = $inv_h -replace "Invite\(ServerDao \\\*server = NULL\);", "Invite(ServerDao *server = NULL, Validator *validator = NULL);"
$inv_h = $inv_h -replace "class Invite : public ACommand \{", "class Validator;\n\nclass Invite : public ACommand {\n private:\n  Validator* m_validator;"
[IO.File]::WriteAllText("include/server/executor/commands/Invite.hpp", $inv_h)

$inv_c = Get-Content "src/server/executor/commands/Invite.cpp" -Raw
$inv_c = $inv_c -replace "Invite::Invite\(ServerDao \\\*server\) : ACommand\(server\) \{\}", "Invite::Invite(ServerDao *server, Validator *validator) : ACommand(server), m_validator(validator) {}"
$inv_c = $inv_c -replace "validator::isValidChannelName", "m_validator->isValidChannelName"
[IO.File]::WriteAllText("src/server/executor/commands/Invite.cpp", $inv_c)

$mode_h = Get-Content "include/server/executor/commands/Mode.hpp" -Raw
$mode_h = $mode_h -replace "Mode\(ServerDao \\\*server = NULL\);", "Mode(ServerDao *server = NULL, ModeReporter *modeReporter = NULL);"
$mode_h = $mode_h -replace "class Mode : public ACommand \{", "class ModeReporter;\n\nclass Mode : public ACommand {\n private:\n  ModeReporter* m_modeReporter;"
[IO.File]::WriteAllText("include/server/executor/commands/Mode.hpp", $mode_h)

$mode_c = Get-Content "src/server/executor/commands/Mode.cpp" -Raw
$mode_c = $mode_c -replace "Mode::Mode\(ServerDao \\\*server\) : ACommand\(server\) \{\}", "Mode::Mode(ServerDao *server, ModeReporter *modeReporter) : ACommand(server), m_modeReporter(modeReporter) {}"
$mode_c = $mode_c -replace "mode_reporter::reportChannelModes", "m_modeReporter->reportChannelModes"
[IO.File]::WriteAllText("src/server/executor/commands/Mode.cpp", $mode_c)
