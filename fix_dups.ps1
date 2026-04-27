$cpp_files = Get-ChildItem "src/server/executor/commands/*.cpp"
foreach ($f in $cpp_files) {
    $c = Get-Content $f.FullName -Raw
    $name = $f.BaseName
    $c = [regex]::Replace($c, "(?m)^$name\:\:$name\(ServerDao \*server\) : ACommand\(server\) \{\}\r?\n?", "\"")
    $c += "\n$name:\:$name(ServerDao *server) : ACommand(server) {}\n\""
    [IO.File]::WriteAllText($f.FullName, $c)
}
$hpp_files = Get-ChildItem "include/server/executor/commands/*.hpp" -Exclude "ACommand.hpp"
foreach ($f in $hpp_files) {
    $c = Get-Content $f.FullName -Raw
    $name = $f.BaseName
    $c = [regex]::Replace($c, "(?m)^[ \t]*$name\(ServerDao \*server = NULL\);\r?\n?", "\"")
    $c = [regex]::Replace($c, "public:", "public:\n  $name(ServerDao *server = NULL);\"")
    [IO.File]::WriteAllText($f.FullName, $c)
}
