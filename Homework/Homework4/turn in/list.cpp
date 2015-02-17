void listAll(const Class* c, string path)  // two-parameter overload
{
    cout << path + c->name() << endl;
    if (c->subclasses().size() == 0)
        return;
    for (auto it = c->subclasses().cbegin(); it != c->subclasses().cend(); it++)
        listAll(*it, path + c->name() + "=>");
}