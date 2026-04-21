1A.
<img width="1569" height="262" alt="1A" src="https://github.com/user-attachments/assets/cd1bcfc7-a438-4655-a265-0817c0c4a5e8" />

1B.
<img width="1555" height="126" alt="1B" src="https://github.com/user-attachments/assets/79388e2b-ae52-4a61-9131-8ec46fbc3c6c" />

2A.
<img width="1506" height="365" alt="2A" src="https://github.com/user-attachments/assets/fa93ed33-9a19-4a9d-9643-e273e8326a2c" />

2B.
<img width="1712" height="357" alt="2B" src="https://github.com/user-attachments/assets/a4e80ed7-cac7-46bd-87b7-8c8cd57ef104" />

3A.
<img width="1359" height="797" alt="3A" src="https://github.com/user-attachments/assets/3270d645-971f-4bf6-a2dd-9c38998d0b12" />

3B.
<img width="1523" height="127" alt="3B" src="https://github.com/user-attachments/assets/55b3cd3d-0bd0-4948-a427-9f2ff4735867" />

4A.
<img width="1702" height="797" alt="4A" src="https://github.com/user-attachments/assets/8a503169-5d40-4489-8b4a-a5bcd788ee95" />

4B.
<img width="1604" height="462" alt="4B" src="https://github.com/user-attachments/assets/86989191-9c13-47a1-abfc-1867b25daa10" />

4C.
<img width="1557" height="157" alt="4C" src="https://github.com/user-attachments/assets/f199b2f3-ad13-4bd7-b2c4-b9a721bb46ca" />

Final 1
<img width="1494" height="964" alt="Final 1" src="https://github.com/user-attachments/assets/60973497-33fb-4dd3-b945-da4ba5b4de02" />

Final 2
<img width="1131" height="1046" alt="Final 2" src="https://github.com/user-attachments/assets/ee0425ff-1afa-44aa-9e10-8a5f98e111fb" />

Final 3
<img width="1226" height="527" alt="Final 3" src="https://github.com/user-attachments/assets/6871d3d5-ef9f-47fd-b44d-912cd87b3d5b" />


Q5.1 — Implementing pes checkout

To implement pes checkout , the repository must update .pes/HEAD and the branch reference under .pes/refs/heads/. If the checkout is to a normal branch, HEAD should continue pointing to the branch name rather than a raw commit hash. The working directory must then be rewritten so it matches the tree of the target commit: files that no longer exist on the target branch must be removed, files that differ must be replaced, and new files and directories from the target branch must be created.

This operation is complex because it must protect user data. Checkout cannot blindly overwrite tracked files if the user has uncommitted local changes. It also has to handle nested directories, deletions, file modes, and untracked files that might be clobbered by the switch.

Q5.2 — Detecting dirty working directory conflicts

A dirty working directory conflict can be detected by comparing the current working directory, the index, and the target branch’s tree. For each tracked path, compare the file in the working directory against the index metadata first. If size or modification time changed, the file may be dirty. For a stronger check, hash the working file and compare it against the blob hash stored in the index. Then compare the target branch’s tree entry for that path with the current staged version.

If a tracked file is modified locally and the target branch has a different version of that same file, checkout should refuse. In other words, checkout must stop whenever switching branches would overwrite uncommitted changes.

Q5.3 — Detached HEAD

Detached HEAD means HEAD stores a commit hash directly instead of a branch reference. Commits made in this state still create valid new commit objects, but no branch pointer is advanced to remember them. The new commits are reachable only through the direct HEAD history until the user switches away.

A user can recover detached-HEAD commits by creating a new branch at the commit hash, or by updating an existing branch reference to that hash. If the hash is still known, the commit history can be preserved without loss.

Q6.1 — Garbage collection and space reclamation

Garbage collection should begin from all branch tips and from HEAD if it points directly to a commit. From those roots, the algorithm recursively traverses commit parents and commit trees, and then each tree’s child blobs and subtrees. Every visited object hash is inserted into a hash set to avoid revisiting the same object multiple times.

After traversal, any object not in the reachable set can be deleted. A hash set is the right data structure because lookups are fast and duplicates are naturally eliminated. With 100,000 commits and 50 branches, the number of objects visited is approximately the number of unique reachable commits plus the trees and blobs they reference. Because branches usually share history, the traversal is closer to the size of the reachable graph than to 50 times the number of commits.

Q6.2 — Why concurrent GC is dangerous

Running garbage collection concurrently with commit creation is dangerous because commit creation is multi-step. A commit may write blobs first, then trees, then the commit object, and finally update the branch ref. If GC runs in the middle, it may see the new blob or tree as unreachable and delete it before the commit object or ref update makes it reachable.

Git avoids this with locking, atomic ref updates, and grace periods before pruning unreachable objects. Recently created or recently referenced objects are not deleted immediately, which prevents GC from racing against in-progress commits.
