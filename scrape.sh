#!/bin/bash
curl() {
	command curl -s "$@"
}
allNames() {
	curl https://pokemondb.net/pokedex/national | sed -e 's/"/\n/g' | sed -ne 's;^/pokedex/;;p' | uniq | awk '/bulbasaur/{P=1} {if (P) print $0}'
}
fetchStats() {
	printf '%s\n' "${HTML[@]}"| awk '/Base stats/{P=1} {if (P) print $0} /<\/table>/{P=0}' | sed -e '/th/N' -e '/th/!d' | sed -ne 's/.*"num"[^0-9]*\([0-9]*\).*/\1/p' 
}
loadHTML() {
	local IFS=$'\n'
	HTML=(`curl https://pokemondb.net/pokedex/"$1"`)
}
(
exec 3> "$1"
exec >&2
cat <<EOF >&3
drop table pokemon;
create table pokemon(
	nid int,
	name varchar(255),
	Hp int,
	Atk int,
	Def int,
	SpA int,
	SpD int,
	Spe int
);
EOF
while read p ; do
	((nid++))
	loadHTML "$p" 
	while : ; do 
		read Hp || break
		read Atk || break
		read Def || break
		read SpA || break
		read SpD || break
		read Spe || break
		#TODO handle other forms (next iteration of loop)
		break
	done < <(fetchStats)
	printf '%03d:%16s\n' $nid $p
	for s in Hp Atk Def SpA SpD Spe ; do
		echo "$s: ${!s}"
	done
cat <<EOF >&3
insert into pokemon values ($nid, '$p', $Hp, $Atk, $Def, $SpA, $SpD, $Spe);
EOF
done < <(allNames)
)
