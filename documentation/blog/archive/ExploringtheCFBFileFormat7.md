<div id="page">

# Exploring the CFB File Format 7

[King Salemno](https://social.msdn.microsoft.com/profile/King%20Salemno)
6/9/2011 9:02:50 PM

-----

<div id="content">

**Exploring the CFB File Format 7**

\-------------------------------

*\[- Red-Black Trees -\]*

As we have seen, OLE Structured Storage files are a series

of storages and streams like a modern day FAT filesystem. One design
goal of a

filesystem in this case is the ability to keep track of the entire
hierarchy of

data. Each storage object and stream object within a compound file has a

directory entry. Thus all data is contained in this tree-like structure.
The

structure itself is comprised of the nature of a red-black tree. In such
a

tree, each node will have a color code, black or red. This particular
tree

implementation in Office files has 3 specific constraints:

1.  The root storage object MUST always be black.
    
    Because the root directory does not have siblings, its color is
    irrelevant and
    
    can therefore be either red or black.

2.  Two consecutive nodes MUST NOT both be red.

3.  The left sibling MUST always be less than the
    
    right sibling. This sorting relationship is defined as follows.

<!-- end list -->

  - A node with a shorter name is less than a node
    
    with a longer name (compare the length of the names from the
    Directory Entry
    
    Name Length field).

  - For nodes with the same name length from
    
    Directory Entry Name Length, iterate through each UTF-16 code point,
    one at a
    
    time, from the beginning of the Unicode string. 

  - For each UTF-16 code point, convert to
    
    upper-case with the Unicode Default Case Conversion Algorithm,
    simple case
    
    conversion variant (simple case foldings), with the following
    notes.\<2\>

Compare each upper-cased UTF-16 code point binary value.

  - Unicode surrogate characters are never
    
    upper-cased, because they are represented by two

UTF-16 code points, while the sorting relationship upper-cases a single

UTF-16 code point at a time.

  - Lowercase characters defined in a newer, later
    
    version of the Unicode standard can be upper-cased by an
    implementation that
    
    conforms to that later Unicode standard.

The compound file producer will save the contents of the

data in the file via the color and sibling attributes. Thus, the
significance

and value of these attributes are decided before data is ultimately
saved on

disk. I will thus present a simple mechanism / app that takes into

consideration the constraints behind the red-black tree in cfb files.
This

mechanism was not created by me; however, it was implemented by Mark C.

Chu-Carroll:

         ***class RedBlackTree(object):***

          

def \_\_init\_\_(self):

            

self.\_tree = None

*** ***

          

def Insert(self, n):

            

if self.\_tree == None:

              

self.\_tree = RedBlackTreeNode(n)

              

self.\_tree.SetColor("Black")

            

else:

              

self.\_tree = self.\_tree.Insert(n)

*** ***

          

def Print(self):

            

if self.\_tree == None:

              

print "Empty"

            

else:

              

self.\_tree.Print(1)

 

Noting the structure above, this is the actual red-black

tree object. Now to represent each
    node:

             class RedBlackTreeNode(object):

               def __init__(self, value):

                 self._left = None

                 self._right = None

                 self._value = value

                 self.SetColor("Red")

                 self._parent = None

``` 
 
```

               def GetParent(self):

                 return self._parent

``` 
 
```

               def SetParent(self, parent):

                   self._parent = parent

``` 
 
```

               def GetColor(self):

                 return self._color

``` 
 
```

               def SetColor(self, color):

                   self._color = color

``` 
 
```

               def GetLeft(self):

                   return self._left

``` 
 
```

               def SetLeft(self, left):

                   self._left = left

``` 
 
```

               def GetRight(self):

                   return self._right

``` 
 
```

               def SetRight(self, right):

                   self._right = right

``` 
 
```

               def GetGrandParent(self):

                 if self.GetParent() != None:

                   return self.GetParent().GetParent()

                 else:

                     return None

``` 
 
```

               def GetUncle(self):

                 grand = self.GetGrandParent()

                 if grand is not None:

                   if grand.GetLeft() == self.GetParent():

                     return grand.GetRight()

                   else:

                     return grand.GetLeft()

                 else:

                   return None

 

               def Rebalance(self):     

                   # WP case 1: tree root

                   if self.GetParent() is None:

                     self.SetColor("Black")

                     return self

                   # WP case 2: The parent of the target node is BLACK,

                   #   so the tree is in fine balance shape; just return the

                   #   tree root

                   if self.GetParent().GetColor() == "Black":

                       return self.GetRoot()

                   # From here on, we know that parent is Red.

                   # WP Case 3:  self, parent, and uncle are all red.

                   if self.GetUncle() is not None and self.GetUncle().GetColor() == "Red":

                       self.GetUncle().SetColor("Black")

                       self.GetParent().SetColor("Black")

                       self.GetGrandParent().SetColor("Red")

                       return self.GetGrandParent().Rebalance()

                   # By now, we know that self and parent are red; and the uncle is black.

                   # We also know that the grandparent is not None, because if it were, the

                   # parent would be root, which must be black. So this means that we 

                   # need to do a pivot on the parent

                   return self.PivotAndRebalance()

``` 
 
```

               def GetRoot(self):

                 if self.GetParent() is None:

                   return self

                 else:

                   return self.GetParent().GetRoot()

``` 
 
```

``` 
 
```

               def PivotAndRebalance(self):

                 # First, distinguish between the case where where my parent is

                 # a left child or a right child.

                 if self.GetGrandParent().GetLeft() == self.GetParent():

                   if self.GetParent().GetRight() == self:

                     # WP case 4: I'm the right child of my parent, and my parent is the

                     # left child of my grandparent. Pivot right around me.

                     return self.PivotLeft(False)

                   else:

                     # WP case 5

                     # If I'm the left child, and my parent is the left child, then

                     # pivot right around my parent.

                     return self.GetParent().PivotRight(True)

                 else: # My parent is the right child.

                   if self.GetParent().GetLeft() == self:

                     # WP case 4, reverse.

                     return self.PivotRight(False)

                   else:

                     # WY case 5 reverse

                     return self.GetParent().PivotLeft(True)

``` 
 
```

``` 
 
```

               def PivotRight(self, recolor):

                   # Hurrah, I'm going to be the new root of the subtree!

                   left = self.GetLeft()

                   right = self.GetRight()

                   parent = self.GetParent()

                   grand = self.GetGrandParent()

                   # move my right child to be the left of my soon-to-be former parent.

                   parent.SetLeft(right)

                   if right is not None:

                       right.SetParent(parent)

                   # Move up, and make my old parent my right child.

                   self.SetParent(grand)

                   if grand is not None:

                     if  grand.GetRight(parent)  == parent:

                       grand.SetRight(self)

                     else:

                       grand.SetLeft(self)

                   self.SetRight(parent)

                   parent.SetParent(self)

                   if recolor is True:

                       parent.SetColor("Red")

                       self.SetColor("Black")

                       return self.GetRoot()

                   else:

                     # rebalance from the new position of my former parent.

                     return parent.Rebalance()

``` 
 
```

               def PivotLeft(self, recolor):

                   # Hurrah, I'm going to be the new root of the subtree!

                   left = self.GetLeft()

                   right = self.GetRight()

                   parent = self.GetParent()

                   grand = self.GetGrandParent()

                   # move my left child to be the right of my soon-to-be former parent.

                   parent.SetRight(left)

                   if left is not None:

                       left.SetParent(parent)

                   # Move up, and make my old parent my right child.

                   self.SetParent(grand)

                   if grand is not None:

                     if  grand.GetRight() == parent:

                       grand.SetRight(self)

                     else:

                       grand.SetLeft(self)

                   self.SetLeft(parent)

                   parent.SetParent(self)

                   if recolor is True:

                     parent.SetColor("Red")

                     self.SetColor("Black")

                     return self.GetRoot()

                   else:

                     # rebalance from the position of my former parent.

                     return parent.Rebalance()

``` 
 
```

``` 
 
```

               def Insert(self, value):

                 if self._value > value:

                   if self.GetLeft() is None:

                     self.SetLeft(RedBlackTreeNode(value))

                     self.GetLeft().SetParent(self)

                     return self.GetLeft().Rebalance()

                   else:

                     return self.GetLeft().Insert(value)

                 else:

                   if self.GetRight() is None:

                     self.SetRight(RedBlackTreeNode(value))

                     self.GetRight().SetParent(self)

                     return self.GetRight().Rebalance()        

                   else:

                     return self.GetRight().Insert(value)

``` 
 
```

               def Print(self, indent):

                 for i in range(indent):

                   print "  ",

                 print "%s (%s)" % (self._value, self.GetColor())

                 if self.GetLeft() is None:

                   for i in range(indent+1):

                     print "  ",

                   print "None(Black)"

                 else:

                   self.GetLeft().Print(indent+1)

                 if self.GetRight() is None:

                   for i in range(indent+1):

                     print "  ",

                   print "None(Black)"

                 else:

                   self.GetRight().Print(indent+1)

 

The actual data needed to create the tree in memory is

already available on disk in the various fields in the relevant
directory

entries. Therefore, it is merely parsing each directory entry and
performing

the insert logic based upon the color of the entry and the entries left
and

right sibling value. Pushing this data back out to disk is merely
traversing

the tree from the root using recursion like any other tree.

 

</div>

</div>
