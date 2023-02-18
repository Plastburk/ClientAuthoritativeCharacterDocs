---
layout: default
---

# What is this?
The built in networking solution in Unreal is generally geared towards giving the server full authority over _everything_; the biggest and most important aspect of this is of course the playable Character's movement.

This plugin reverses that idea, giving each Client full authority over their own Character's movement.

# Why would I want this?
There are several pros and cons of both approaches, and which one is most appropriate for your game depends a lot on what type of game you're making.

### No jarring corrections from the server
In networked games, you will always have to live with a bit of lag, desyncs and poor synchronization. More so if your players have subpar internet connections.
This leads to discrepancies in what a Client sees on their screen compared to what the Server sees, and it's your job as the developer to hide this as well as possible.

So what happens when things don't quite line up? Well, with Server Authoritative movement, the server will see that the Client has done something different, which it deems incorrect, and will then send a correction to the Client to get it back to the correct location. These corrections are very jarring for the player when they happen as you suddenly start teleporting to correct the error, which a user will see as significant lag.

These types of corrections are of course unavoidable in networked games. But if the Client is Authoritative over their own movement, **they** wont ever get any corrections, and what they see on the screen will be what actually happens for their character.

Instead, those corrections to your own character will only be seen by other players, which is **significantly** easier to hide with some smoothing. Additionally, since the other players that see your character wont be controlling it themselves, they likely wont even notice any discrepancies, as it can be very hard to tell the difference between what they saw happen and what the controlling Client saw.

### Significantly easier to make custom movement modes
Since we now basically just replicate position and velocity, creating custom movement modes is **significantly** easier.

Normally we'd have to make sure that the Server simulates the character exactly the same way the Client did, which is not a trivial task.
It requires adding new data to replicate and save, as well as making sure that the new movement logic works with Unreal's networking rewind and replay system, etc.
All this requires deep knowledge about how Unreal's C++ movement replication works, and even then it takes a lot of tinkering and iterating to get everything working smoothly.
This is especially true for really unique and complex movement modes that are vastly different from the normal walking mode.

With a Client Authoritative approach, we can essentially modify the movement however we want, similar to how we could do it in a single player game.
Anything from building custom MovementComponents, to just directly modifying the Character's location and velocity in Blueprints will work perfectly, without having to consider complicated replication issues.

### Cheating
The drawback is of course that the Server is no longer Authoritative, which theoretically makes it a lot easier for Clients to cheat and develop teleportation hacks.

But this is only really a problem if you're making a truly competitive game, in which case you probably shouldn't be using this plugin.

# How to use
The plugin is built to be a drop in replacement for the default Character and PlayerController classes. As such, setting it up and using it is incredibly simple.

## Blueprints
Simply open your Character blueprint, and replace the parent class from **Character** to **Client Authoritative Character**:
![CharacterClass](./assets/images/Character.png)

Then do the same to your PlayerController blueprint, replacing the parent class from **Player Controller** to **Client Authoritative Player Controller**:
![ControllerClass](./assets/images/Controller.png)

And that's it, you're done!

## C++
For C++, you do the exact same thing as with blueprints above, but in C++:

Replace the parent class of your Character from **ACharacter** to **AClientAuthoritativeCharacter**, and the parent class of your Controller from **APlayerController** to **AClientAuthoritativePlayerController**.

If you use a custom MovementComponent, you will also need to replace its parent class from **UCharacterMovementComponent** to **UClientAuthoritativeCharacterMovementComponent**.

And that's it, you're done!

# Extras

## Gameplay Ability System (Optional)
If your game is using GAS to create abilities, especially abilities that include some kind of movement (dashes, completely custom movement logic, etc), [Epic recommends that you use Root Motion Sources](https://forums.unrealengine.com/t/a-holistic-look-at-replicated-movement/265041) to handle this, as it allows them to tie into Unreal's movement replication. With Client Authoritative characters, this is much less of a requirement, but it can still be a very useful way to create custom movement modes as part of abilities.

It does however require quite a large amount of setup and boilerplate code, most of it things that are irrelevant for Client Authoritative characters.
This plugin therefore includes (fully optional) AbilityTask and RootMotionSource base-classes to help creating these. I have provided an example Task using these that make the Character walk on a provided spline, which should help get you started with using them: [.h](Link) [.cpp](Link).

If you are not using GAS, or are not using RootMotionSources already; **Don't worry, they are in no way required to use this plugin!** This is simply an extra addition for anyone who would like to use it.