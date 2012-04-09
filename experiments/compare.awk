{
  if (FILENAME == results) { new[$1]=$0; newval[$1]=$3 }
  else { old[$1]=$0; oldval[$1]=$3 }
}
END {
  for (ind in old) {
    oldline=old[ind]; newline=new[ind];
    if (oldline == newline) continue;
    if (newval[ind] > oldval[ind]) change="++";
    else if (newval[ind] == oldval[ind]) change="==";
    else change="--";
    print change, oldline;
    print change, newline;
    print "";
  }
}
