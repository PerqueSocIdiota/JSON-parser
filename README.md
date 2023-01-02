# JSON-parser
Simple JSON parser for C++

## Motivation
I was working in another project which needed some way to save and manage data. From the available alternatives I decided to use JSON, as it was simple and readable.
Then, the motivation behind this project is to implement a simple JSON library that can be used in another projects.

### Why not just use some already existing library instead of reinventing the wheel?
Good question. The main reason is because I want to be in control of my code:
 * It can happen that open-source libraries use licences that are not clear enough, or are too restrictive for the project I have in mind.
 * I want to know what my code does, without it being bloated with functions I don't really need.
 * It's possible that open-source solutions rely on relatively complex tools and scripts to build the project. When they work fine, that's fantastic. But when they don't work it becomes a nightmare.
 By implementing my own solution, I can reduce the number of dependencies and potentially avoid or
 reduce such problems.
 * As a side effect, I can learn about how to implement and manage a library. Such knowledge could
 be useful in the future.

Those might not be good reasons at all. At the end I will waste more time implementing my own solution than any time I would spend integrating a third-party library into my project. Not to mention that the quality of my code would probably be mediocre at best. But... doing my own thing will be more entertaining, so that's what I will do.

## Scope
The implementation is not meant to be fast, efficient or reliable. I will try to make it functional, but I won't exhaustively test it. I won't try to improve it either, I just want to implement some basic functionality and then focus on something else.

I won't provide any kind of support or maintainance. This is not a collaborative project. There will probably be bugs, but I won't fix them unless they affect me.
I don't want to sound rude but I neither want to give false expectations. At the end, what I want is to put my effort into other projects: this is just a tool, a small step, to help me towards the completion of other bigger projects.

In summary, this is provided "as is". If you find it useful you can use it, fork it, modify it, etc.
You can try to contact me, but I can't ensure I will be available to respond.
