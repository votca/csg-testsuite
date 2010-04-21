#! /bin/bash
npol=15;
echo "<cg_molecule>
        <name>P3HT</name>
        <ident>Protein</ident>
        <topology>
             <cg_beads> "
for i  in `seq 1 $((npol ))`  
do 
	j=$(( $i - 1)) 
	sed -e 's/NUMBER/'${i}'/g' -e 's/POSN/'$j'/' singlebead.fo ; done
echo "             </cg_beads> "
echo "             <cg_bonded> "
echo "			<bond>"
echo "				<name>AA-bond</name>"
echo "				<beads>"

for i in `seq 1 $(($npol-1)) `
do 
echo "			A${i} A$(($i+1))"
done
echo "				</beads>"
echo "			</bond>"


echo "			<angle>"
echo "				<name>AAA-angle</name>"
echo "				<beads>"
for i in `seq 1 $(($npol-2)) `
do 
echo "			A${i} A$(($i+1)) A$(($i + 2))"
done
echo "				</beads>"
echo "			</angle>"

echo "			<dihedral>"
echo "				<name>AAAA-dihedral</name>"
echo "				<beads>"
for i in `seq 1 $(($npol-3)) `
do 
echo "			A${i} A$(($i+1)) A$(($i + 2)) A$(( $i + 3))"
done
echo "				</beads>"
echo "			</dihedral>"

echo "		</cg_bonded>"
echo "	</topology>"
echo " 	<maps>"
echo "		<map>"
echo "			<name>A</name>"
echo "			<weights> 1. 1. 1.  1. 1. 1.</weights>"
echo "		</map>"
echo "	</maps>"
echo "</cg_molecule>"
