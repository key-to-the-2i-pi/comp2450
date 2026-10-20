# Trial I — *The Keepers' Brief*

*Warden of the Middle Gates · Midterm 2 · 15 %*

Four short answers, one for each floor span above. **50–100 words per answer.** No code on this trial (except the one-liner in Q2); just clear thinking.

AI is welcome to *check* your answers; it is not welcome to *write your sentences*. These answers are short enough that an LLM voice is recognisable.

---

## 1. Floors 4 / 4½ — linked lists

> Your encounter's **event log** (every "Aric strikes Imp for 6", every "Imp retaliates for 3") is appended to `hero.eventLog`, which is a `Chain<std::string>`. Defend that against a `Bag<std::string>` (vector-backed) in terms of one operation the battle actually performs — naming the Big-O of that operation in `Chain` and in `Bag`. *Hint: think about what `Bag` has to do the moment its capacity is exceeded.*

(your answer — 50–100 words)
How I understand it, a Bag has a set size that can grow to a certain threshold, but the moment that threshold is exceeded, it has to recalibrate and change its size to compensate for the new information, but a Chain is able to just tag along to either end (depending on how they set it up) very quickly, allowing the Chain to be much quicker for adding things than a Bag would be.

---

## 2. Floor 5 — iterators

> When the player picks the **Inspect log** menu option, your encounter calls `printLog(hero.eventLog.begin(), hero.eventLog.end(), 5, ...)` — the same function template your Floor 5 iterators brought to life. (Because `eventLog` is built with `push_front` everywhere, the *front* of the chain is the most recent entry; the forward iterator walks newest-to-oldest naturally.) Show **one line** of C++ that does the equivalent walk by hand using an explicit `Chain<std::string>::iterator` loop. Then one sentence: why is this iterator *bidirectional* (and what becomes impossible the moment you ask for random-access on it)?

```cpp
// your one-line walk
for (Chain<std::string>::iterator item = hero.eventLog.begin(); item != hero.eventLog.end(); item++) {std::cout << "Item: " << item << "\n";}
```

This iterator is bidirectional because we can access the hero.eventLog.begin() and the hero.eventLog.end(), but we lose the speed the moment we decide to have random-access on it (I think). I'm pretty sure the iterator allows us to have the ability to iterate through the contents of the Chain using just an index, of which we lose the moment we ask for random-access.

---

## 3. Floor 6 — stacks

> Your `undo` command pops the most recent action off a `Stack<UndoAction>` and restores its snapshot. Defend the choice of stack against a queue or a deque in one sentence — referencing the *contract* of LIFO (not the implementation). Then one extra sentence: name one piece of battle state that your `undo` *intentionally does not* restore, and explain why the player won't mind.

(your answer — 50–100 words)
From what the purpose of an Undo function is supposed to do, it wouldn't make sense to use a Queue. With a Queue, when you perform an action, that action is stored at the front, and then each subsequent action will be placed at the back, but whenever you want to read() from this list, you get the front of the list instead of the back, which is where the proper order starts. You want to use a stack() instead so that it can have the proper contract. 

Whenever you run the Undo command, it doesn't restore the fact that you just ran an undo, hense why the player wouldn't mind that you restored it.

---

## 4. Floor 7 — queues

> Each wave's enemies sit in a `Queue<Enemy>` in arrival order. Explain — in one sentence — what *visibly* changes for the player if you swap `Queue<Enemy>` for `Stack<Enemy>`: which enemy gets attacked first, and in what order the rest are picked off.

(your answer — one or two sentences)
The thing that visibly changes whenever you change a Queue<Enemy> into a Stack<Enemy> is that the monster that was at the back is now at the front, and the next to last is second to first, and so on. Whenever you attack an enemy, it gets knocked out of the front of the list and the next in line gets moved up. 