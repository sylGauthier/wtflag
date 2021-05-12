# wtflag: a generic pretty-printer for registers

wtflag takes a register description defined in `templates/` and a register
value, and will print the status of every flag for the given reg value and the
reg description.

## Install

Edit the `Makefile`, in particular `PREFIX` to set the install directory, then
type:

```
$ make install
```

Not that when changing the `PREFIX` you should `make clean` first.

## Usage

```
$ wtflag <regname> <hexvalue> [<hexvalue>]
```

Will pretty-print the register's flags based on the descriptor file and the
first hex value. You can pass a second, optional hexvalue to get a diff of
flags.

## Register descriptor file format

The format is straight-forward, each line should match the following format:

```
n1[-n2]: <name>
```

Where `n1` is the bit index, the optional `n2` preceded by a `-` is the second
bound of the field if the register's field is multi-bit.

## Example

```
$ wtflag armv8/tcr_el1 0x32b5503510
```

returns:

```
RES0 [63-60]:_____________0000
DS [59-59]:_______________0
TCMA1 [58-58]:____________0
TCMA0 [57-57]:____________0
E0PD1 [56-56]:____________0
E0PD0 [55-55]:____________0
NFD1 [54-54]:_____________0
NFD0 [53-53]:_____________0
TBID1 [52-52]:____________0
TBID0 [51-51]:____________0
HWU162 [50-50]:___________0
HWU161 [49-49]:___________0
HWU160 [48-48]:___________0
HWU159 [47-47]:___________0
HWU062 [46-46]:___________0
HWU061 [45-45]:___________0
HWU060 [44-44]:___________0
HWU059 [43-43]:___________0
HPD1 [42-42]:_____________0
HPD0 [41-41]:_____________0
HD [40-40]:_______________0
HA [39-39]:_______________0
TBI1 [38-38]:_____________0
TBI0 [37-37]:_____________1
AS [36-36]:_______________1
IPS [34-32]:______________010
TG1 [31-30]:______________10
SH1 [29-28]:______________11
ORGN1 [27-26]:____________01
IRGN1 [25-24]:____________01
EPD1 [23-23]:_____________0
A1 [22-22]:_______________1
T1SZ [21-16]:_____________010000
TG0 [15-14]:______________00
SH0 [13-12]:______________11
ORGN0 [11-10]:____________01
IRGN0 [9-8]:______________01
EPD0 [7-7]:_______________0
T0SZ [5-0]:_______________010000
```
