if test "$#" -lt 1; then
    echo "Provide a parameter - customer waiting time" >&2
    exit 1
fi
x=$1
./customer.o >/dev/null $x &
./customer.o >/dev/null $x &
./customer.o >/dev/null $x &
./customer.o $x
