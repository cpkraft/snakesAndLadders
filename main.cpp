#include <map>
#include <cstdint>
#include <string>
#include <iostream>
#include <cassert>

struct Portal
{
   enum class Type { Snake, Ladder };
   Type type;
   uint32_t target;
};

struct Action
{
   enum class Type { Move, Win, NoMove };

   Action(Type actionType, uint32_t square) { type = actionType; target = square; }

   Type type;
   uint32_t target;

   bool operator==(const Action& rhs) const
   {
      return std::tie(type, target) == std::tie(rhs.type, rhs.target);
   }
};

// First test: don't write anything in board, just a stub class - even this has a test
// i.e. a test for each line of code is a good starting point
class Board
{
   public:
      Action landOn(uint32_t square) const
      {
         auto finalSquare = [&]
         {
            auto it = portals.find(square);
            if (it != portals.end())
               return it->second.target; // board square with snake/portal
            return square;
         }();

         if (finalSquare == size) return Action(Action::Type::Win, size);               // landed on final square FTW!
         else if (finalSquare > size) return Action(Action::Type::NoMove, finalSquare); // overshot final square
         else return Action(Action::Type::Move, finalSquare);                           // default: move to empty square (i.e. no snake/portal)
      }

   private:
      static const uint32_t size = 100;
      static const std::map<uint32_t, Portal> portals;
};

// test naming is really important.
//  acts documentation for code and tests.
//  acts as communiction for someone who's not technical or familiar with the code base.

// Board can tell us what action to perform.
//  Don't want to test the map and the unit test.
//  we want to test the board.
// first failing test: try to instantiate a board object for which the class hasn't been written.
// Next: what can we do with this board? what happens if we land on a particular square?... assume it returns the square we land on.
//  So first write a stub for landOn in board.
//  progressively update code as you rapidly go between red and green.
// Next we want to start associating the board with the snakes/ladders on the board map itself.
//  so we write a test for a case where the position you land on results in the end of a snake/ladder, e.g. 2->38.
//   This test fails, obviously, so now we need to consider the portals, while retaining the behavior of the empty squares.
//    Take this opportunity to move the map into board as static const to Board class.
//     Comment out failing test as well to ensure the code compiles first. This was the first good refactor.
// Next to make the test pass, update landOn method to iterate through portals to find a snake/ladder and return the target if found, else return square for an empty square
// Next add test to check the first snake
//  we expect this to pass though, so technically not part of the TDD process, but it's good.
// Next interesting test is going to be the winning square.
//  we expect it to return 100, but is that what we want?
//    who's responsibility is it to know it's the winnign square and tell us we've won? The board.
// Add test for square 100, to return Action-Win
//  At this point we're going to return Action from landOn
//   Action is going to look like Portal, Type:Move/Win, type, target
//    Since this isn't the refactoring step, the minimum work possible for the test is to change each call to landOn to pull out the target for the comparison
//     So we're not considering if it's a move or win yet
//      refactor? yes, now have the right side of the assert compare to an action
// This fails b/c Action doesn't have a comparison operator
// So implement operator== for Action
//  After all said and done got a GREEN again
// Slight refactor to only check for type on the landOn(100) test instead of the entire action
//  still pass, green to green
// Refactor to make size=100 a static const property of the board
// Refactor to return Action objects directly rather than carrying around one in landOn
// Still green
// Next square 80 is a ladder to a winning move, so add a new test for that
// Update landOn to account for this
//  We can use a immediately invoked lambda expression here (eseentially a function within a function) to return the finalSquare
//   Then add a conditon at the end to return the correction Action
//    Retest and we're GREEN
// Final test: what happens if we ask for a square beyond the end
//  Add failing test for landOn(101) checking type is NoMove
//   Add new state: no move
//   Add an extra case in landOn
//    Now we're GREEN again
// Lastly we need to give names to each of the assertions b/c they are effectively separate tests and need good names: *** TODO ***
void testBoard()
{
   Board board;
   assert(board.landOn(1) == Action(Action::Type::Move, 1));
   assert(board.landOn(3) == Action(Action::Type::Move, 3));
   assert(board.landOn(2) == Action(Action::Type::Move, 38));
   assert(board.landOn(47) == Action(Action::Type::Move, 26));
   assert(board.landOn(100).type == Action::Type::Win);
   assert(board.landOn(80) == Action(Action::Type::Win, 100));
   assert(board.landOn(101).type == Action::Type::NoMove);
}

// move this static const here just for the sake of ease of visibility for test code
const std::map<uint32_t, Portal> Board::portals =
{
   { 2, { Portal::Type::Ladder, 38 } },
   { 4, { Portal::Type::Ladder, 14 } },
   { 8, { Portal::Type::Ladder, 31 } },
   { 21, { Portal::Type::Ladder, 42 } },
   { 28, { Portal::Type::Ladder, 84 } },
   { 36, { Portal::Type::Ladder, 44 } },
   { 47, { Portal::Type::Snake, 26 } },
   { 49, { Portal::Type::Snake, 11 } },
   { 51, { Portal::Type::Ladder, 67 } },
   { 56, { Portal::Type::Snake, 53 } },
   { 62, { Portal::Type::Snake, 18 } },
   { 64, { Portal::Type::Snake, 60 } },
   { 71, { Portal::Type::Ladder, 91 } },
   { 80, { Portal::Type::Ladder, 100 } },
   { 87, { Portal::Type::Snake, 24 } },
   { 93, { Portal::Type::Snake, 73 } },
   { 95, { Portal::Type::Snake, 75 } },
   { 98, { Portal::Type::Snake, 75 } },
};

// chosen unsigned int so don't have to worry about negatives
// best test is one you didn't have to write
// roll dice to progress on board linearly
// ladders called "portals" more abstractly
// first to square 100 wins and you have to land exactly on 100
main()
{
   testBoard();
}