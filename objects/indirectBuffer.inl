namespace magma
{
    template<class DrawCommandType>
    class DrawIndirectCommand final
    {
    public:
        explicit DrawIndirectCommand(IndirectBuffer *buffer, void *mappedData) noexcept:
            buffer(buffer)
        {
            MAGMA_ASSERT(buffer->getDrawCount() < buffer->getMaxDrawCount());
            if (buffer->persistentlyMapped())
                drawCmd = reinterpret_cast<DrawCommandType *>(mappedData) + buffer->getDrawCount();
            else
            {
                drawCmd = buffer->getMemory()->map<DrawCommandType>(
                    sizeof(DrawCommandType) * buffer->getDrawCount(),
                    sizeof(DrawCommandType));
                MAGMA_ASSERT(drawCmd);
            }
        }

        ~DrawIndirectCommand()
        {
            if (!buffer->persistentlyMapped())
                buffer->getMemory()->unmap();
        }

        DrawCommandType *operator->() { return drawCmd; }
        DrawCommandType& operator*() { return *drawCmd; }

        operator bool() const { return drawCmd != nullptr; }

    private:
        IndirectBuffer *const buffer;
        DrawCommandType *drawCmd;
    };
} // namespace magma
